#pragma once

#include <future>
#include <cinttypes>
#include <exception>
#include <functional>
#include <optional>
#include <string>
#include <curl/curl.h>

#include <OpenAI.h>
#include <OpenAIModelType.h>
#include <OpenAIEndpoint.h>
#include <ImageGeneration.h>

//import Foundation
//#if canImport(FoundationNetworking) && canImport(FoundationXML)
//import FoundationNetworking
//import FoundationXML
//#endif

class OpenAIError : public std::exception {};

class OpenAIGenericError : public OpenAIError
{};

class OpenAIDecodingError : public OpenAIError
{};

struct Config {
public:
    /// Initialiser
    /// - Parameter session: the session to use for network requests.
    Config(CURL* curl = nullptr) : curl(curl) {
        if (curl == nullptr) {
            curl = curl_easy_init();
        }
    }

    CURL* curl;
};

class OpenAIHelper {
public:
    std::string token;
    Config config;
    
    /// Configuration object for the client

    
    OpenAIHelper(std::string authToken, Config config = Config()) 
        : token(authToken), config(config) {};

    /// Send a Completion to the OpenAI API
    /// - Parameters:
    ///   - prompt: The Text Prompt
    ///   - model: The AI Model to Use. Set to `OpenAIModelType.gpt3(.davinci)` by default which is the most capable model
    ///   - maxTokens: The limit character for the returned response, defaults to 16 as per the API
    ///   - completionHandler: Returns an OpenAI Data Model
    void sendCompletion(std::string prompt,
                        std::function<void(std::optional<OpenAI<TextResult>>, std::optional<OpenAIError>)> completionHandler,
                        OpenAIModelType modelType = OpenAIModelType::gpt3_davinci,
                        uint32_t maxTokens = 16, 
                        double temperature = 1.0) {
        /*
        auto endpoint = Endpoint.completions
        let body = Command(prompt: prompt, model: model.modelName, maxTokens: maxTokens, temperature: temperature)
        let request = prepareRequest(endpoint, body: body)
        
        makeRequest(request: request) { result in
            switch result {
            case .success(let success):
                do {
                    let res = try JSONDecoder().decode(OpenAI<TextResult>.self, from: success)
                    completionHandler(.success(res))
                } catch {
                    completionHandler(.failure(.decodingError(error: error)))
                }
            case .failure(let failure):
                completionHandler(.failure(.genericError(error: failure)))
            }
        }
        */        
    }
    
    /// Send a Edit request to the OpenAI API
    /// - Parameters:
    ///   - instruction: The Instruction For Example: "Fix the spelling mistake"
    ///   - model: The Model to use, the only support model is `text-davinci-edit-001`
    ///   - input: The Input For Example "My nam is Adam"
    ///   - completionHandler: Returns an OpenAI Data Model
    void sendEdits( std::string instruction,
                    std::function<void(std::optional<OpenAI<TextResult>>, std::optional<OpenAIError>)> completionHandler,
                    OpenAIModelType model = OpenAIModelType::feature_davinci,
                    std::string input = "") {
        /*
        let endpoint = Endpoint.edits
        let body = Instruction(instruction: instruction, model: model.modelName, input: input)
        let request = prepareRequest(endpoint, body: body)
        
        makeRequest(request: request) { result in
            switch result {
            case .success(let success):
                do {
                    let res = try JSONDecoder().decode(OpenAI<TextResult>.self, from: success)
                    completionHandler(.success(res))
                } catch {
                    completionHandler(.failure(.decodingError(error: error)))
                }
            case .failure(let failure):
                completionHandler(.failure(.genericError(error: failure)))
            }
        }
        */
    }
    
    /// Send a Chat request to the OpenAI API
    /// - Parameters:
    ///   - messages: Array of `ChatMessages`
    ///   - model: The Model to use, the only support model is `gpt-3.5-turbo`
    ///   - user: A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    ///   - temperature: What sampling temperature to use, between 0 and 2. Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic. We generally recommend altering this or topProbabilityMass but not both.
    ///   - topProbabilityMass: The OpenAI api equivalent of the "top_p" parameter. An alternative to sampling with temperature, called nucleus sampling, where the model considers the results of the tokens with top_p probability mass. So 0.1 means only the tokens comprising the top 10% probability mass are considered. We generally recommend altering this or temperature but not both.
    ///   - choices: How many chat completion choices to generate for each input message.
    ///   - stop: Up to 4 sequences where the API will stop generating further tokens.
    ///   - maxTokens: The maximum number of tokens allowed for the generated answer. By default, the number of tokens the model can return will be (4096 - prompt tokens).
    ///   - presencePenalty: Number between -2.0 and 2.0. Positive values penalize new tokens based on whether they appear in the text so far, increasing the model's likelihood to talk about new topics.
    ///   - frequencyPenalty: Number between -2.0 and 2.0. Positive values penalize new tokens based on their existing frequency in the text so far, decreasing the model's likelihood to repeat the same line verbatim.
    ///   - logitBias: Modify the likelihood of specified tokens appearing in the completion. Maps tokens (specified by their token ID in the OpenAI Tokenizer—not English words) to an associated bias value from -100 to 100. Values between -1 and 1 should decrease or increase likelihood of selection; values like -100 or 100 should result in a ban or exclusive selection of the relevant token.
    ///   - completionHandler: Returns an OpenAI Data Model
    void sendChat(  std::vector<ChatMessage> messages,
                    std::function<void(std::optional<OpenAI<MessageResult>>, std::optional<OpenAIError>)> completionHandler,
                    OpenAIModelType model = OpenAIModelType::chat_chatgpt,
                    std::optional<std::string> user = nullptr,
                    std::optional<double> temperature = 1,
                    std::optional<double> topProbabilityMass = 0,
                    std::optional<uint32_t> choices = 1,
                    std::optional<std::vector<std::string>*> stop = nullptr,
                    std::optional<uint32_t> maxTokens = 100,
                    std::optional<double> presencePenalty = 0,
                    std::optional<double> frequencyPenalty = 0,
                    std::optional<std::unordered_map<int, double>*> logitBias = nullptr){
        /*
        let endpoint = Endpoint.chat
        let body = ChatConversation(user: user,
                                    messages: messages,
                                    model: model.modelName,
                                    temperature: temperature,
                                    topProbabilityMass: topProbabilityMass,
                                    choices: choices,
                                    stop: stop,
                                    maxTokens: maxTokens,
                                    presencePenalty: presencePenalty,
                                    frequencyPenalty: frequencyPenalty,
                                    logitBias: logitBias)

        let request = prepareRequest(endpoint, body: body)
        
        makeRequest(request: request) { result in
            switch result {
                case .success(let success):
                    do {
                        let res = try JSONDecoder().decode(OpenAI<MessageResult>.self, from: success)
                        completionHandler(.success(res))
                    } catch {
                        if let resp = String(data: success, encoding: .utf8) {
                            print("Failed to decode response:\n", resp)
                        }
                        completionHandler(.failure(.decodingError(error: error)))
                    }
                case .failure(let failure):
                    completionHandler(.failure(.genericError(error: failure)))
            }
        }
        */
    }

    /// Send a Image generation request to the OpenAI API
    /// - Parameters:
    ///   - prompt: The Text Prompt
    ///   - numImages: The number of images to generate, defaults to 1
    ///   - size: The size of the image, defaults to 1024x1024. There are two other options: 512x512 and 256x256
    ///   - user: An optional unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    ///   - completionHandler: Returns an OpenAI Data Model
    void sendImages( std::string prompt,
                    std::function<void(std::optional<OpenAI<UrlResult>>, std::optional<OpenAIError>)> completionHandler,
                    uint32_t numImages = 1,
                    ImageSize::Size size = ImageSize::size256,
                    std::optional<std::string> user = nullptr){
        /*
        let endpoint = Endpoint.images
        let body = ImageGeneration(prompt: prompt, n: numImages, size: size, user: user)
        let request = prepareRequest(endpoint, body: body)

        makeRequest(request: request) { result in
            switch result {
                case .success(let success):
                    do {
                        let res = try JSONDecoder().decode(OpenAI<UrlResult>.self, from: success)
                        completionHandler(.success(res))
                    } catch {
                        completionHandler(.failure(.decodingError(error: error)))
                    }
                case .failure(let failure):
                    completionHandler(.failure(.genericError(error: failure)))
                }
        }
        */
    }

private:
    void makeRequest(   CURL* request, 
                        std::function<void(std::optional<char*> data, std::optional<uint32_t> byteCount)> completionHandler) {
        /*
        let session = config.session
        let task = session.dataTask(with: request) { (data, response, error) in
            if let error = error {
                completionHandler(.failure(error))
            } else if let data = data {
                completionHandler(.success(data))
            }
        }
        
        task.resume()
        */
    }
    
    template<typename BodyType>
    CURL* prepareRequest( Endpoint _endpoint, BodyType body ) {
        return curl_easy_init();
        /*
        var urlComponents = URLComponents(url: URL(string: endpoint.baseURL())!, resolvingAgainstBaseURL: true)
        urlComponents?.path = endpoint.path
        var request = URLRequest(url: urlComponents!.url!)
        request.httpMethod = endpoint.method
        
        if let token = self.token {
            request.setValue("Bearer \(token)", forHTTPHeaderField: "Authorization")
        }
        
        request.setValue("application/json", forHTTPHeaderField: "content-type")
        
        let encoder = JSONEncoder()
        if let encoded = try? encoder.encode(body) {
            request.httpBody = encoded
        }
        
        return request
        */
    }

public:

    /// Send a Completion to the OpenAI API
    /// - Parameters:
    ///   - prompt: The Text Prompt
    ///   - model: The AI Model to Use. Set to `OpenAIModelType.gpt3(.davinci)` by default which is the most capable model
    ///   - maxTokens: The limit character for the returned response, defaults to 16 as per the API
    ///   - temperature: Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic. Defaults to 1
    /// - Returns: Returns an OpenAI Data Model
    //@available(swift 5.5)
    //@available(macOS 10.15, iOS 13, watchOS 6, tvOS 13, *)
    std::future<OpenAI<TextResult>> sendCompletion( std::string prompt,
                                                    OpenAIModelType model = OpenAIModelType::gpt3_davinci,
                                                    uint32_t maxTokens = 16,
                                                    double temperature = 1.0)  {
        auto asyncFunction = [this](std::string prompt, OpenAIModelType model, uint32_t maxTokens, double temperature) -> OpenAI<TextResult> {
            std::promise<OpenAI<TextResult>> resultValue;
            auto future = resultValue.get_future();
            auto completionHandler = [&](std::optional<OpenAI<TextResult>> result, std::optional<OpenAIError> error) {
                if (error.has_value()) {
                    throw error.value();
                }
                resultValue.set_value(result.value());
            };

            sendCompletion(prompt, completionHandler, model, maxTokens, temperature);
            return future.get();
        };
        
        return std::async(std::launch::async, asyncFunction, prompt, model, maxTokens, temperature);
    }
    
    /// Send a Edit request to the OpenAI API
    ///
    /// - Parameters:
    ///   - instruction: The Instruction For Example: "Fix the spelling mistake"
    ///   - model: The Model to use, the only support model is `text-davinci-edit-001`
    ///   - input: The Input For Example "My nam is Adam"
    /// - Returns: Returns an OpenAI Data Model
    // @available(swift 5.5)
    // @available(macOS 10.15, iOS 13, watchOS 6, tvOS 13, *)
    std::future<OpenAI<TextResult>> sendEdits(std::string instruction, OpenAIModelType model = OpenAIModelType::feature_davinci, std::string input = "") {
        
        auto asyncFunction = [this](std::string instruction, OpenAIModelType model, std::string input) -> OpenAI<TextResult> {
            std::promise<OpenAI<TextResult>> resultValue;
            auto future = resultValue.get_future();
            auto completionHandler = [&](std::optional<OpenAI<TextResult>> result, std::optional<OpenAIError> error){
                if (error.has_value()) {
                    throw error.value();
                }
                resultValue.set_value(result.value());
            };

            sendEdits(instruction, completionHandler, model, input);

            return future.get();
        };
        
        return std::async(std::launch::async, asyncFunction, instruction, model, input);
    }
    
    /// Send a Chat request to the OpenAI API
    /// - Parameters:
    ///   - messages: Array of `ChatMessages`
    ///   - model: The Model to use, the only support model is `gpt-3.5-turbo`
    ///   - user: A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    ///   - temperature: What sampling temperature to use, between 0 and 2. Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic. We generally recommend altering this or topProbabilityMass but not both.
    ///   - topProbabilityMass: The OpenAI api equivalent of the "top_p" parameter. An alternative to sampling with temperature, called nucleus sampling, where the model considers the results of the tokens with top_p probability mass. So 0.1 means only the tokens comprising the top 10% probability mass are considered. We generally recommend altering this or temperature but not both.
    ///   - choices: How many chat completion choices to generate for each input message.
    ///   - stop: Up to 4 sequences where the API will stop generating further tokens.
    ///   - maxTokens: The maximum number of tokens allowed for the generated answer. By default, the number of tokens the model can return will be (4096 - prompt tokens).
    ///   - presencePenalty: Number between -2.0 and 2.0. Positive values penalize new tokens based on whether they appear in the text so far, increasing the model's likelihood to talk about new topics.
    ///   - frequencyPenalty: Number between -2.0 and 2.0. Positive values penalize new tokens based on their existing frequency in the text so far, decreasing the model's likelihood to repeat the same line verbatim.
    ///   - logitBias: Modify the likelihood of specified tokens appearing in the completion. Maps tokens (specified by their token ID in the OpenAI Tokenizer—not English words) to an associated bias value from -100 to 100. Values between -1 and 1 should decrease or increase likelihood of selection; values like -100 or 100 should result in a ban or exclusive selection of the relevant token.
    ///   - completionHandler: Returns an OpenAI Data Model
    //@available(swift 5.5)
    //@available(macOS 10.15, iOS 13, watchOS 6, tvOS 13, *)
    std::future<OpenAI<MessageResult>> sendChat(
                        std::vector<ChatMessage> messages,
                        OpenAIModelType model = OpenAIModelType::chat_chatgpt,
                        std::optional<std::string> user = nullptr,
                        std::optional<double> temperature = 1.0,
                        std::optional<double> topProbabilityMass = 0.0,
                        std::optional<uint32_t> choices = 1,
                        std::optional<std::vector<std::string>*> stop = nullptr,
                        std::optional<uint32_t> maxTokens = 100,
                        std::optional<double> presencePenalty = 0.0,
                        std::optional<double> frequencyPenalty = 0.0,
                        std::optional<std::unordered_map<int, double>*> logitBias = nullptr) {
        auto asyncFunction = [&](std::vector<ChatMessage> messages,
                                    OpenAIModelType model,
                                    std::optional<std::string> user,
                                    std::optional<double> temperature,
                                    std::optional<double> topProbabilityMass,
                                    std::optional<uint32_t> choices,
                                    std::optional<std::vector<std::string>*> stop,
                                    std::optional<uint32_t> maxTokens,
                                    std::optional<double> presencePenalty,
                                    std::optional<double> frequencyPenalty,
                                    std::optional<std::unordered_map<int, double>*> logitBias){
            std::promise<OpenAI<MessageResult>> resultValue;
            auto future = resultValue.get_future();
            auto completionHandler = [&](std::optional<OpenAI<MessageResult>> result, std::optional<OpenAIError> error){
                if (error.has_value()) {
                    throw error.value();
                }
                resultValue.set_value(result.value());
            };
            sendChat(messages, completionHandler, model, user, temperature, topProbabilityMass, choices, stop, maxTokens, presencePenalty, frequencyPenalty, logitBias);

            return future.get();
        };

        return std::async(std::launch::async, asyncFunction,
                            messages,
                            model,
                            user,
                            temperature,
                            topProbabilityMass,
                            choices,
                            stop,
                            maxTokens,
                            presencePenalty,
                            frequencyPenalty,
                            logitBias);
    }


    /// Send a Image generation request to the OpenAI API
    /// - Parameters:
    ///   - prompt: The Text Prompt
    ///   - numImages: The number of images to generate, defaults to 1
    ///   - size: The size of the image, defaults to 1024x1024. There are two other options: 512x512 and 256x256
    ///   - user: An optional unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    /// - Returns: Returns an OpenAI Data Model
    // @available(swift 5.5)
    // @available(macOS 10.15, iOS 13, watchOS 6, tvOS 13, *)
    std::future<OpenAI<UrlResult>> sendImages(std::string prompt,
                                              uint32_t numImages,
                                              ImageSize::Size size = ImageSize::size1024,
                                              std::optional<std::string> user = nullptr) {
        auto asyncFunction = [this](std::string prompt, uint32_t numImages, ImageSize::Size size, std::optional<std::string> user) -> OpenAI<UrlResult> {
            std::promise<OpenAI<UrlResult>> resultValue;
            auto future = resultValue.get_future();
            auto completionHandler = [&](std::optional<OpenAI<UrlResult>> result, std::optional<OpenAIError> error){
                if (error.has_value()) {
                    throw error.value();
                }
                resultValue.set_value(result.value());
            };
            sendImages(prompt, completionHandler, numImages, size, user);
            return future.get();
        };
        
        return std::async(std::launch::async, asyncFunction, prompt, numImages, size, user);
    }
};

