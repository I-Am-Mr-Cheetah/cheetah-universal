#pragma once

#include <OpenAIModelType.h>

#include <string>
#include <sstream>
#include <unordered_map>

enum class ModelInputType
{
    prompt,
    messages,
    chatPrompt
};

struct ModelInput{
    std::string systemMessage;
    std::string prompt;
    OpenAIModelType model;
    std::string user;
    ModelInputType type;
};

enum class ContextKey {
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
    default:
        std::ostringstream oss;
        oss << "invalid value: " << std::hex << int(key) << std::endl;
        throw std::runtime_error(std::string(oss.str()));
    }
}


typedef std::unordered_map<ContextKey, std::string> AnalysisContext;
