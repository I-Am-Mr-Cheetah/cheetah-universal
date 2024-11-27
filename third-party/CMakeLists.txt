
add_subdirectory(whisper.cpp)

# Adapted from SDL guidance regarding CMake integration:
# Create an option to switch between a system sdl library and a vendored SDL library

option(USE_SDL_VENDORED "Use vendored SDL library" ON)
if(USE_SDL_VENDORED)
    add_subdirectory(SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_subdirectory(openai-cpp)
