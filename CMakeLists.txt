cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

project(cheetah-universal VERSION 0.1 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(third-party)

