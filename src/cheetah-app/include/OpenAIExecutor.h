#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <OpenAIHelper.h>
#include <QtLogging>

/*
enum ModelInput 
{
    case prompt(String, model: OpenAIModelType.GPT3 = .davinci)
    case messages([ChatMessage], model: OpenAIModelType.Chat = .gpt4)
    case chatPrompt(system: String, user: String, model: OpenAIModelType.Chat = .gpt4)
}
*/

struct ModelInput{
    std::string systemMessage;
    std::string prompt;
    std::string model;
    std::string user;
};

class PromptChain {
    // <Context>
    std::function<std::string(ModelInput&)> generator;
    std::function<void(AnalysisContext& context, ContextKey key, std::string& str)> updateContext;
    uint32_t maxTokens;
    std::shared_ptr<std::vector<PromptChain>> children;
    
    PromptChain(std::function<std::string(ModelInput)> generator,
        std::function<void(AnalysisContext& context, ContextKey key, std::string& str)> updateContext,
        uint32_t maxTokens = 16,
        std::shared_ptr<std::vector<PromptChain>> children = nullptr
    ) {
        this->generator = generator;
        this->updateContext = updateContext;
        this->maxTokens = maxTokens;
        this->children = children;
    }
};

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
    
    void log(std::string& prompt) {
        // TODO: add an analog for Swift's UserDefaults
        //if (UserDefaults.standard.logPrompts) {
            qInfo("Prompt:\n", prompt);
        //}
    }
    
    void log(std::string& completion) {
        // TODO: add an analog for Swift's UserDefaults
        //if UserDefaults.standard.logCompletions {
            qInfo("Completion:\n", completion);
        //}
    }
    
    std::optional<std::string> execute(std::string& prompt, OpenAIModelType& model, uint32_t maxTokens = 100) {
        log(prompt: prompt)
        auto result = openAI.sendCompletion(prompt, model, maxTokens);
        auto text = result.get().choices.hasValue() ? result.get()..first?.text;
        if (text) {
            log(text);
        } else if (error.hasValue()) {
            throw error.value();
        }
        return text
    }
    
    func execute(messages: [ChatMessage], model: OpenAIModelType, maxTokens: Int = 100) async throws -> String? {
        log(prompt: messages.debugDescription)
        let result = try await openAI.sendChat(with: messages, model: model, maxTokens: maxTokens)
        let content = result.choices?.first?.message.content
        if let content = content {
            log(completion: content)
        } else if let error = result.error {
            throw error
        }
        return content
    }
    
    func adjustModel(_ model: OpenAIModelType.Chat) -> OpenAIModelType.Chat {
        if !useGPT4 && model == .gpt4 {
            return .chatgpt
        } else {
            return model
        }
    }
    
    func execute<K>(chain: PromptChain<[K: String]>, context initialContext: [K: String]) async throws -> [K: String] {
        var context = initialContext
        
        guard let input = try chain.generator(context) else {
            return context
        }
        
        let output: String?
        switch input {
        case .prompt(let prompt, let model):
            output = try await execute(prompt: prompt, model: .gpt3(model), maxTokens: chain.maxTokens)
            
        case .messages(let messages, let model):
            output = try await execute(messages: messages, model: .chat(adjustModel(model)), maxTokens: chain.maxTokens)
            
        case .chatPrompt(system: let systemMessage, user: let userMessage, model: let model):
            let messages = [
                ChatMessage(role: .system, content: systemMessage),
                ChatMessage(role: .user, content: userMessage),
            ]
            output = try await execute(messages: messages, model: .chat(adjustModel(model)), maxTokens: chain.maxTokens)
        }
        
        guard let output = output else {
            return context
        }
        
        try chain.updateContext(String(output.trimmingCharacters(in: .whitespacesAndNewlines)), &context)
        
        let childContext = context
        
        if let children = chain.children {
            let childOutputs = try await withThrowingTaskGroup(
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
        }
        
        return context
    }
}
