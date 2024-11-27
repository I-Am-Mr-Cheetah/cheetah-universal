#pragma once

#include <string>
#include <stdexcept>

//
//  OpenAIModelType.h
//  
//
//  Created by Yash Shah on 06/12/2022.
//  Modified
//

/// The type of model used to generate the output
enum OpenAIModelType {
    /// ``GPT3`` Family of Models
    gpt3_davinci,
    gpt3_curie,
    gpt3_babbage,
    gpt3_ada,   
    /// ``Codex`` Family of Models
    codex_davinci,
    codex_cushman,
    /// ``Feature`` Family of Models
    feature_davinci,
    /// ``Chat`` Family of Models
    chat_chatgpt,
    chat_chatgpt0301,
    chat_gpt4,
    chat_gpt4_0314,
    chat_gpt4_32k,
    chat_gpt4_32k_0314,
    /// Other Custom Models
    other,
    invalid,
};

std::string GetModelTypeName(OpenAIModelType modelType) {
    switch (modelType) {
        case OpenAIModelType::gpt3_davinci:         return "text-davinci-003";
        case OpenAIModelType::gpt3_curie:           return "text-curie-001";
        case OpenAIModelType::gpt3_babbage:         return "text-babbage-001";
        case OpenAIModelType::gpt3_ada:             return "text-ada-001";
        case OpenAIModelType::codex_davinci:        return "code-davinci-002";
        case OpenAIModelType::codex_cushman:        return "code-cushman-001";
        case OpenAIModelType::feature_davinci:      return "text-davinci-edit-001";
        case OpenAIModelType::chat_chatgpt:         return "gpt-3.5-turbo";
        case OpenAIModelType::chat_chatgpt0301:     return "gpt-3.5-turbo-0301";
        case OpenAIModelType::chat_gpt4:            return "gpt-4";
        case OpenAIModelType::chat_gpt4_0314:       return "gpt-4-0314";
        case OpenAIModelType::chat_gpt4_32k:        return "gpt-4-32k";
        case OpenAIModelType::chat_gpt4_32k_0314:   return "gpt-4-32k-0314";
        case OpenAIModelType::other:                return "other";
        default:
            throw std::invalid_argument("Invalid Model Type");
            return "invalid";
    }
};

