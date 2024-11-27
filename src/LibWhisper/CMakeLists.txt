cmake_minimum_required(VERSION 3.16)

set(LibWhisper_Version_Major 0)
set(LibWhisper_Version_Minor 1)
add_compile_definitions(
    LIBWHISPER_VERSION_MAJOR=${LibWhisper_Version_Major}
    LIBWHISPER_VERSION_MINOR=${LibWhisper_Version_Minor}
)

# Set the project name
project(LibWhisper)

# Set the source files
set(SOURCE_FILES 
        stream.cpp
        common.cpp
        stream.cpp
        WhisperStream.cpp)

# Add the library
add_library(LibWhisper STATIC ${SOURCE_FILES}
    include/CaptureDevice.h)

# Link against the SDL2 library
target_link_libraries(LibWhisper
                        PUBLIC SDL3::SDL3
                        PRIVATE whisper)

# Specify the include directories
target_include_directories(LibWhisper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
