#pragma once

//
//  Created by Adam Rush - OpenAISwift
//

#include <stdexcept>


enum Endpoint {
    completions,
    edits,
    chat,
    images
};

namespace EndpointUtils {
    const char * getPath(Endpoint endpoint) {
        switch (endpoint){
            case Endpoint::completions:
                return "/v1/completions";
            case Endpoint::edits:
                return "/v1/edits";
            case Endpoint::chat:
                return "/v1/chat/completions";
            case Endpoint::images:
                return "/v1/images/generations";
            default:
                throw std::invalid_argument("Invalid Endpoint");
        }
    }
    
    const char* getMethod(Endpoint endpoint) {
        switch (endpoint) {
            case Endpoint::completions: 
            case Endpoint::edits:
            case Endpoint::chat: 
            case Endpoint::images:
                return "POST";
            default:
                throw std::invalid_argument("Invalid Endpoint");
        }
    }
    
    const char* getBaseURL(Endpoint endpoint) {
        switch (endpoint) {
            case Endpoint::completions: 
            case Endpoint::edits:
            case Endpoint::chat: 
            case Endpoint::images:
                return "https://api.openai.com";
            default:
                throw std::invalid_argument("Invalid Endpoint");
        }
    }
}
