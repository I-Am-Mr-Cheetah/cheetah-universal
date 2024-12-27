#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <OpenAIHelper.h>
#include <PromptGenerator.h>
#include <PromptChain.h>
#include <ConversationAnalyzer.h>
#include <QtLogging>

typedef PromptChain Prompt;

/* //TODO: Implement UserDefault values for logging prompt and completions
extension UserDefaults {
    @objc var logPrompts: Bool {
        get {
            bool(forKey: "logPrompts")
        }
        set {
            set(newValue, forKey: "logPrompts")
        }
    }
    
    @objc var logCompletions: Bool {
        get {
            bool(forKey: "logCompletions")
        }
        set {
            set(newValue, forKey: "logCompletions")
        }
    }
}
*/

class OpenAIExecutor {
public:    
    // TODO: OpenAISwift is a swift class, we need to reimplement it in C++ using LibOpenAI
    
    OpenAIHelper openAI;
    bool useGPT4 = true;
    
    OpenAIExecutor(OpenAIHelper openAI, bool useGPT4) 
        : openAI(openAI), useGPT4(useGPT4) {};
    
    OpenAIExecutor(std::string& authToken, bool useGPT4) 
        : openAI(OpenAIHelper(authToken)), useGPT4(useGPT4) {};
    
    void logPrompt(std::string& prompt) {
        // TODO: add an analog for Swift's UserDefaults
        //if (UserDefaults.standard.logPrompts) {
            qInfo("Prompt:\n%s", prompt.c_str());
        //}
    }
    
    void logCompletion(std::string& completion) {
        // TODO: add an analog for Swift's UserDefaults
        //if UserDefaults.standard.logCompletions {
            qInfo("Completion:\n%s", completion.c_str());
        //}
    }
    
    std::optional<std::string> executePrompt(std::string& prompt, OpenAIModelType& model, uint32_t maxTokens = 100) {
        logPrompt(prompt);
        auto result = openAI.sendCompletion(prompt, model, maxTokens);
        auto text = result.get().choices.value()[0].text;

        logCompletion(text);
        return text;
    }
    
    std::optional<std::string> executeMessages(std::vector<ChatMessage>& messages, OpenAIModelType& model, uint32_t maxTokens = 100) {
        for (auto message : messages){
            logPrompt(message.content);
        }
        auto result = openAI.sendChat(messages, model, nullptr, 1.0, 0.0, 1, nullptr, 100);
        auto content = result.get().choices.value()[0].message.content;
        logCompletion(content);

        return content;
    }
    
    /*
    OpenAIModelType adjustModel(OpenAIModelType.Chat) -> OpenAIModelType.Chat {
        if !useGPT4 && model == .gpt4 {
            return .chatgpt
        } else {
            return model
        }
    }
    */
      
    std::vector<std::string> execute(PromptChain chain, AnalysisContext& initialContext) {
        auto context = initialContext;
        
        auto input = chain.generator(context[ContextKey::question]);

        std::optional<std::string> output;

        switch (input.type) {
        case ModelInputType::prompt:
            output = executePrompt(context[ContextKey::question], input.model, chain.maxTokens);
            break;
        case ModelInputType::messages:
            {
                std::vector<ChatMessage> messages;
                messages.emplace_back(ChatRole::Role::user, context[ContextKey::transcript]);

                output = executeMessages(messages, input.model, chain.maxTokens);
            }
            break;
        case ModelInputType::chatPrompt:
            {
                std::vector<ChatMessage> messages = {
                    ChatMessage(ChatRole::Role::system, PromptGenerator::systemMessage()),
                    ChatMessage(ChatRole::Role::user, context[ContextKey::question]),
                };
                output = executeMessages(messages, input.model, chain.maxTokens);
            }
            break;
        }
        
        //chain.updateContext(output, &context);
        
        auto childContext = context;

        if (chain.children.get() != nullptr) {
            /*
            auto childOutputs = try await withThrowingTaskGroup(
                of: [K: String?].self,
                returning: [K: String?].self
            ) { group in
                for child in children {
                    group.addTask {
                        return try await self.execute(chain: child, context: childContext)
                    }
                }
                
                return try await group.reduce(into: [:]) {
                    for (key, output) in $1 {
                        $0[key] = output
                    }
                }
            }
            
            for (key, output) in childOutputs {
                context[key] = output
            }
            */
        }

        return {output.value()};
    }
};
