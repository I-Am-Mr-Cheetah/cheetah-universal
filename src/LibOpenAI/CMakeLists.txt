cmake_minimum_required(VERSION 3.16)

set(LibOpenAI_Version_Major 0)
set(LibOpenAI_Version_Minor 1)
add_compile_definitions(
    LIBOPENAI_VERSION_MAJOR=${LibOpenAI_Version_Major}
    LIBOPENAI_VERSION_MINOR=${LibOpenAI_Version_Minor}
)

find_package(CURL REQUIRED)

# Set the project name
project(LibOpenAI)

# Set the source files
set(SOURCE_FILES
        LibOpenAI.cpp)

# Add the library
add_library(LibOpenAI STATIC ${SOURCE_FILES}
    )

# Link against the CURL library
target_link_libraries(LibOpenAI
                        PUBLIC CURL::libcurl)

# Specify the include directories
target_include_directories(LibOpenAI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
