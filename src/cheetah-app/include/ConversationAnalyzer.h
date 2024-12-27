#pragma once

#include <BrowserExtension.h>
#include <LibWhisper.h>
#include <ModelInput.h>
#include <OpenAIExecutor.h>
#include <PromptGenerator.h>
#include <PromptChain.h>
#include <WhisperStream.h>

#include <cctype>
#include <cstdarg>
#include <exception>
#include <functional>
#include <future>
#include <regex>
#include <optional>
#include <string>
#include <unordered_map>

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

const std::regex finalAnswerRegex("Final answer:\n", std::regex_constants::ECMAScript);
const std::regex answerOnlyRegex("\n\n", std::regex_constants::ECMAScript);

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
public:
    AnalysisPromptGenerator() : PromptGenerator(){}
    AnalysisPromptGenerator(const PromptGenerator& other) : PromptGenerator(other){}
    AnalysisPromptGenerator(const AnalysisPromptGenerator& other) : PromptGenerator(other){}
    AnalysisPromptGenerator(const AnalysisPromptGenerator&& other) : PromptGenerator(other){}


    virtual std::optional<ModelInput> extractQuestion(AnalysisContext context) {
        if (!context.contains(ContextKey::transcript)) {
            return PromptGenerator::extractQuestion(context[ContextKey::transcript]);
        } else {
            throw MissingRequiredContextKeyAnalysisError(ContextKey::transcript);
        }
    }

    virtual std::optional<ModelInput> answerQuestion(AnalysisContext context) {
        if (!context.contains(ContextKey::question)) {
            throw MissingRequiredContextKeyAnalysisError(ContextKey::question);
        }
        auto question = context[ContextKey::question];
        if (context.contains(ContextKey::answerInCode)) {
            return {};
        } else if (context.contains(ContextKey::previousAnswer) && !context[ContextKey::previousAnswer].empty()) {
            return answerPreviousQuestion(question, context[ContextKey::previousAnswer]);
        } else if (context.contains(ContextKey::highlightedAnswer) && !context[ContextKey::highlightedAnswer].empty()) {
            return answerHighlightedQuestion(question, context[ContextKey::highlightedAnswer]);
        } else {
            return answerPromptQuestion(question);
        }
    }

    virtual std::optional<ModelInput> writeCode(AnalysisContext context) {
        if (context.contains(ContextKey::answerInCode) && context.contains(ContextKey::question)) {
            auto question = context[ContextKey::question];
            return PromptGenerator::writeCode(question);
        } else {
            return {};
        }
    }

    virtual std::optional<ModelInput> analyzeBrowserCode(AnalysisContext context) {
        if (context.contains(ContextKey::browserCode) && !context[ContextKey::browserCode].empty()
            && context.contains(ContextKey::browserLogs) && !context[ContextKey::browserLogs].empty()
            && context.contains(ContextKey::question) && !context[ContextKey::question].empty()) {
            auto code = context[ContextKey::browserCode];
            auto logs = context[ContextKey::browserLogs];
            auto task = context[ContextKey::question];
            return PromptGenerator::analyzeBrowserCode(code, logs, task);
        } else {
            return {};
        }
    }
};

namespace ContextHelpers{
    void extractQuestionUpdateContext (std::string output, AnalysisContext& context) {
        auto stringItr = output.begin();
        auto stringItrEnd = output.end();
        const std::string extractQuestionString("Extracted question: ");
        output.find(extractQuestionString);
        //const std::regex extractQuestionRegex ("/Extracted question: (?<question>[^\n]+)(?:\nAnswer in code: (?<answerInCode>Yes|No))?/.ignoresCase()", std::regex_constants::ECMAScript | std::regex_constants::icase);
        //TODO: implement string parsing
        //auto regexItr = std::sregex_iterator(output.begin(), output.end(), extractQuestionRegex);
        //auto regexEndItr = std::sregex_iterator();
        //if (regexItr != regexEndItr){
        //    auto match = *regexItr;

        //}
        //if let match = output.firstMatch(of: regex) {
        //    context[.question] = String(match.question)
        //    if let answerInCode = match.answerInCode {
        //        context[.answerInCode] = String(answerInCode)
        //    }
        //}
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
                        :   stream(std::move(stream)),
                            generator(std::move(generator)),
                            executor(std::move(executor)) {}

    ConversationAnalyzer(ConversationAnalyzer& other)
        :   stream(std::move(other.stream)),
            generator(other.generator),
            executor(other.executor) {}

    AnalysisContext context;

    std::future<void> answer(std::string::const_iterator selection_begin, std::string::const_iterator selection_end, bool refine = false) {
        /*
        PromptChain chain (
            std::bind(AnalysisPromptGenerator::extractQuestion, generator),
            ContextHelpers::extractQuestionUpdateContext,
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
        */
        return std::future<void>();
    }

    void analyzeCode(BrowserExtensionState extensionState) {
        /*
        AnalysisContext newContext(
            [](OrderedSegments& segments) -> std::string {
                std::string result;
                for(auto val : segments){
                    result += val.text + "\n";
                };
                return result;
            }(stream.getSegments()),
            extensionState.getCodeDescription(),
            extensionState.getLogsDescription()
        );
        */
        /*
        auto chain = PromptChain(
            generator.extractQuestion,
            extractQuestion,
            250,
            {
                Prompt(generator.analyzeBrowserCode,
                       ContextKey.answer.set,
                       500)
            }
        );
        */
        //context = try await executor.execute(chain: chain, context: newContext)
    }
};
