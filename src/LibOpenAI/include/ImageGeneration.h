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

namespace ImageSize{
    enum Size {
        size1024,
        size512,
        size256,
    };
}

struct ImageGeneration {
    std::string prompt;
    uint32_t n;
    ImageSize::Size size;
    std::optional<std::string> user;
};



namespace ImageSizeUtils {
    const char* getSize(ImageSize::Size size) {
        switch (size) {
            case ImageSize::Size::size1024:
                return "1024x1024";
            case ImageSize::Size::size512:
                return "512x512";
            case ImageSize::Size::size256:
                return "256x256";
            default:
                throw std::invalid_argument("Invalid Image Size");
        }
    }
}
