#pragma once

//
//  ImageGeneration.swift
//  
//
//  Created by Arjun Dureja on 2023-03-11.
//

//import Foundation

#include <string>
#include <optional>
#include <cstdint>
#include <stdexcept>

struct ImageGeneration {
    std::string prompt;
    uint32_t n;
    ImageSize size;
    std::optional<std::string> user;
};

enum ImageSize {
    size1024,
    size512,
    size256,
};

namespace ImageSizeUtils {
    const char* getSize(ImageSize size) {
        switch (size) {
            case ImageSize::size1024:
                return "1024x1024";
            case ImageSize::size512:
                return "512x512";
            case ImageSize::size256:
                return "256x256";
            default:
                throw std::invalid_argument("Invalid Image Size");
        }
    }
}
