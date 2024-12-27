#pragma once

#include <functional>
#include <memory>

#include <ModelInput.h>

class PromptChain {
    // <Context>
public:
    std::function<ModelInput&(std::string)> generator;
    std::function<void(AnalysisContext&, ContextKey, std::string&)> updateContext;
    uint32_t maxTokens;
    std::shared_ptr<std::vector<PromptChain>> children;

    PromptChain(std::function<ModelInput&(std::string)> generator,
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
