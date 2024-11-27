#pragma once
//
//  File.swift
//
//
//  Created by Bogdan Farca on 02.03.2023.
//

#include <string>
#include <vector>
#include <optional>
#include <cinttypes>
#include <unordered_map>

//mport Foundation


/// An enumeration of possible roles in a chat conversation.
enum ChatRole {
    /// The role for the system that manages the chat interface.
    system,
    /// The role for the human user who initiates the chat.
    user,
    /// The role for the artificial assistant who responds to the user.
    assistant,
};

/// A structure that represents a single message in a chat conversation.
struct ChatMessage {
    /// The role of the sender of the message.
    ChatRole role;
    /// The content of the message.
    std::string content;

    /// Creates a new chat message with a given role and content.
    /// - Parameters:
    ///   - role: The role of the sender of the message.
    ///   - content: The content of the message.
    ChatMessage(ChatRole role, std::string content) : role(role), content(content) {}
};

/// A structure that represents a chat conversation.
struct ChatConversation {
    /// The name or identifier of the user who initiates the chat. Optional if not provided by the user interface.
    std::string user;

    /// The messages to generate chat completions for. Ordered chronologically from oldest to newest.
    std::vector<ChatMessage> messages;

    /// The ID of the model used by the assistant to generate responses. See OpenAI documentation for details on which models work with the Chat API.
    std::string model;

    /// A parameter that controls how random or deterministic the responses are, between 0 and 2. Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic. Optional, defaults to 1.
    std::optional<double> temperature;

    /// A parameter that controls how diverse or narrow-minded the responses are, between 0 and 1. Higher values like 0.9 mean only the tokens comprising the top 90% probability mass are considered, while lower values like 0.1 mean only the top 10%. Optional, defaults to 1.
    std::optional<double> topProbabilityMass;

    /// How many chat completion choices to generate for each input message. Optional, defaults to 1.
    std::optional<uint32_t> choices;

    /// An array of up to 4 sequences where the API will stop generating further tokens. Optional.
    std::optional<std::vector<std::string>> stop;

    /// The maximum number of tokens to generate in the chat completion. The total length of input tokens and generated tokens is limited by the model's context length. Optional.
    std::optional<uint32_t> maxTokens;

    /// A parameter that penalizes new tokens based on whether they appear in the text so far, between -2 and 2. Positive values increase the model's likelihood to talk about new topics. Optional if not specified by default or by user input. Optional, defaults to 0.
    std::optional<double> presencePenalty;

    /// A parameter that penalizes new tokens based on their existing frequency in the text so far, between -2 and 2. Positive values decrease the model's likelihood to repeat the same line verbatim. Optional if not specified by default or by user input. Optional, defaults to 0.
    std::optional<double> frequencyPenalty;

    /// Modify the likelihood of specified tokens appearing in the completion. Maps tokens (specified by their token ID in the OpenAI Tokenizer—not English words) to an associated bias value from -100 to 100. Values between -1 and 1 should decrease or increase likelihood of selection; values like -100 or 100 should result in a ban or exclusive selection of the relevant token.
    std::unordered_map<int, double> logitBias;

    enum CodingKeys {
        user,
        messages,
        model,
        temperature,
        topProbabilityMass,
        choices,
        stop,
        maxTokens,
        presencePenalty,
        frequencyPenalty,
        logitBias,
    };

    const char* to_string(CodingKeys keys){
        switch(keys){
            case CodingKeys::user: return "";
            case CodingKeys::messages: return "";
            case CodingKeys::model: return "";
            case CodingKeys::temperature: return "";
            case CodingKeys::topProbabilityMass: return "top_p";
            case CodingKeys::choices: return "n";
            case CodingKeys::stop: return "";
            case CodingKeys::maxTokens: return "max_tokens";
            case CodingKeys::presencePenalty: return "presence_penalty";
            case CodingKeys::frequencyPenalty: return "frequency_penalty";
            case CodingKeys::logitBias: return "logit_bias";
        }
    }
}
