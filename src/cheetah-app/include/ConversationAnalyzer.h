#pragma once

#include <LibWhisper.h>

#include <cctype>
#include <cstdarg>
#include <exception>
#include <functional>
#include <regex>
#include <unordered_map>
#include <string>

#include <PromptGenerator.h>

enum ContextKey {
    transcript,
    question,
    answerInCode,
    answer,
    previousAnswer,
    highlightedAnswer,
    codeAnswer,
    browserCode,
    browserLogs,
};

const char* to_string(ContextKey key) {
    switch (key) {
        case ContextKey::transcript: return "transcript";
        case ContextKey::question: return "question";
        case ContextKey::answerInCode: return "answerInCode";
        case ContextKey::answer: return "answer";
        case ContextKey::previousAnswer: return "previousAnswer";
        case ContextKey::highlightedAnswer: return "highlightedAnswer";
        case ContextKey::codeAnswer: return "codeAnswer";
        case ContextKey::browserCode: return "browserCode";
        case ContextKey::browserLogs: return "browserLogs";
    }
}

typedef std::unordered_map<ContextKey, std::string> AnalysisContext;

bool doAnswerInCode (AnalysisContext context) {
    if (context.find(ContextKey::answerInCode) != context.end()) {
        return false;
    }
    auto answerInCodeString = context[ContextKey::answerInCode];
    if (answerInCodeString.empty()) {
        return false;
    }
    return std::tolower(answerInCodeString[0]) == 'y';
}

// these helper functions aren't necessary, due to the unordered_map's contract, however this is how the original Swift implementation chose to implement it.
void AnalysisContextSet(AnalysisContext& context, ContextKey key, std::string& string) {
    context[key] = string;
}

std::function<void(AnalysisContext& context, ContextKey key, std::string& str)> extract(std::vector<std::regex> regexes) {
    return [regexes](AnalysisContext& context, ContextKey key, std::string& output) {
        for (auto regex : regexes) {
            auto words_begin = std::sregex_iterator(output.begin(), output.end(), regex);
            auto words_end = std::sregex_iterator();
            if (words_begin != words_end) {
                context[key] = words_begin->str();
            }
        }
    };
}

const std::regex finalAnswerRegex("/Final answer:\n(?<answer>[-•].+$)/", std::regex_constants::ECMAScript);
const std::regex answerOnlyRegex("/(?<answer>[-•].+$)/", std::regex_constants::ECMAScript);

class AnalysisError : public std::exception {
public:
    AnalysisError() : std::exception(){};
};

class MissingRequiredContextKeyAnalysisError : public AnalysisError {
public:
    MissingRequiredContextKeyAnalysisError(ContextKey key) : AnalysisError(){};
    ContextKey key;
};

class AnalysisPromptGenerator : public PromptGenerator {
    virtual std::optional<ModelInput> extractQuestion(AnalysisContext context) {
        if (context.hasKey(ContextKey::transcript)) {
            return PromptGenerator::extractQuestion(context[ContextKey::transcript]);
        } else {
            throw MissingRequiredContextKeyAnalysisError(to_string(ContextKey::transcript));
        }
    }

    virtual std::optional<ModelInput> answerQuestion(AnalysisContext context) {
        if (!context.hasKey(ContextKey::question)) {
            throw MissingRequiredContextKeyAnalysisError(to_string(ContextKey::question));
        }
        auto question = context[ContextKey::question];
        if (answerInCode(context) {
            return nullptr;
        } else if (context.hasKey(ContextKey::previousAnswer) && !context[ContextKey::previousAnswer].empty()) {
            return answerQuestion(question, context[ContextKey::previousAnswer]);
        } else if (context.hasKey(ContextKey::highlightedAnswer) && !context[ContextKey::highlightedAnswer].empty()) {
            return answerQuestion(question, context[ContextKey::highlightedAnswer]);
        } else {
            return answerQuestion(question);
        }
    }

    virtual std::optional<ModelInput> writeCode(AnalysisContext context) {
        if (answerInCode(context) && context.hasKey(ContextKey::question)) {
            auto question = context[ContextKey::question];
            return writeCode(task: question);
        } else {
            return nullptr;
        }
    }

    virtual std::optional<ModelInput> analyzeBrowserCode(AnalysisContext context) {
        if (context.hasKey(ContextKey::browserCode) && !context[ContextKey::browserCode].empty()
            && context.hasKey(ContextKey::browserLogs) && !context[ContextKey::browserLogs].empty()
            && context.hasKey(ContextKey::question) && !context[ContextKey::question].empty()) {
            auto code = context[ContextKey::browserCode];
            auto logs = context[ContextKey::browserLogs];
            auto task = context[ContextKey::question];
            return analyzeBrowserCode(code, logs, task);
        } else {
            return nullptr;
        }
    }
}

class ConversationAnalyzer {
public:
    WhisperStream stream;
    AnalysisPromptGenerator generator;
    OpenAIExecutor executor;

    ConversationAnalyzer(WhisperStream stream, 
                        PromptGenerator generator,
                        OpenAIExecutor executor) 
                        :   stream(stream), 
                            generator(generator),
                            executor(executor) {}

    AnalysisContext context;

    std::future<void> answer(bool refine = false, std::iterator<std::string> selection_begin = nullptr, std::iterator<std::string> selection_end = nullptr) {
        PromptChain chain (
            generator.extractQuestion,
            extractQuestion,
            250,
            {
                PromptChain(generator.answerQuestion,
                       extract(finalAnswerRegex, answerOnlyRegex),
                       500),
                PromptChain(generator.writeCode,
                       AnalysisContextSet,
                       1000),
            });

        AnalysisContext newContext (
            .transcript: std::string(stream.segments.text)
        );

        if refine, let previousAnswer = context[.answer] {
            if let selection = selection {
                let highlightedAnswer = previousAnswer[..<selection.lowerBound] + " [start highlighted text] " + previousAnswer[selection] + " [end highlighted text] " + previousAnswer[selection.upperBound...]
                newContext[.highlightedAnswer] = String(highlightedAnswer)
            } else {
                newContext[.previousAnswer] = previousAnswer
            }
        }

        context = try await executor.execute(chain: chain, context: newContext)
    }

    func analyzeCode(extensionState: BrowserExtensionState) async throws {
        let newContext: AnalysisContext = [
            .transcript: String(stream.segments.text),
            .browserCode: extensionState.codeDescription,
            .browserLogs: extensionState.logsDescription
        ]

        let chain = PromptChain(
            generator: generator.extractQuestion,
            updateContext: extractQuestion,
            maxTokens: 250,
            children: [
                Prompt(generator: generator.analyzeBrowserCode,
                       updateContext: ContextKey.answer.set,
                       maxTokens: 500)
            ])

        context = try await executor.execute(chain: chain, context: newContext)
    }
}
