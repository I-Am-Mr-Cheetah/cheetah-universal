#pragma once

#include <string>
#include <cinttypes>
#include <stdexcept>
#include <vector>
#include <optional>

#include <ChatMessage.h>

//
//  Created by Adam Rush - OpenAISwift
//

//public protocol Payload: Codable { } // Codable is a type that is serializable.
// TODO: possibly add serialization support for these structs.

struct ErrorResult {
    std::string message;
};

namespace CodingKeys {
enum Tokens {
    promptTokens,
    completionTokens,
    totalTokens,
};
};

struct Payload {};

struct TextResult : public Payload {
    std::string text;
};

struct MessageResult : public Payload {
    ChatMessage message;
};

struct UrlResult : public Payload {
    std::string url;
};

struct UsageResult : public Payload {
    uint32_t promptTokens;
    uint32_t completionTokens;
    uint32_t totalTokens;

    const char * to_string(CodingKeys::Tokens key) {
        switch(key){
        case CodingKeys::Tokens::promptTokens:      return "prompt_tokens";
        case CodingKeys::Tokens::completionTokens:  return "completion_tokens";
        case CodingKeys::Tokens::totalTokens:       return "total_tokens";
        default:
            throw std::invalid_argument("Invalid Coding Key");
        }
    }
};

template <typename T>
struct OpenAI {
    std::optional<std::string>      object;
    std::optional<std::string>      model;
    std::optional<std::vector<T>>   choices;
    std::optional<UsageResult>      usage;
    std::optional<std::vector<T>>   data;
    std::optional<ErrorResult>      error;
};
