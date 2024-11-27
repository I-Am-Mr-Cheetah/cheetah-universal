#pragma once

#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>

class CaptureDeviceError : public std::runtime_error {

public:
    CaptureDeviceError(const std::string error_description) : std::runtime_error(error_description){};
    virtual const char* what() const noexcept
    {
        std::stringstream ss;
        ss << "An exception occured when initializing SDL Audio Capture device. Error Details: " << std::runtime_error::what();

        char* output = new char[ss.str().size()];
        std::strncpy(output, ss.str().c_str(), ss.str().size());

        return output;
    }
};

struct CaptureDevice {
    int32_t id;
    std::string name;

    CaptureDevice (int32_t id, std::string name) : id(id), name(name) {}

    static std::vector<CaptureDevice> devices;

    static std::vector<CaptureDevice>& get_devices()
    {
        if (devices.empty())
        {
            auto result = SDL_Init(SDL_INIT_AUDIO);
            if (!result)
            {
                throw new CaptureDeviceError(SDL_GetError());
            }

            int sdl_audio_recording_device_count{};
            SDL_AudioDeviceID* sdl_audio_recording_device_ids = SDL_GetAudioRecordingDevices(&sdl_audio_recording_device_count);
            for (size_t i = 0; i < sdl_audio_recording_device_count;  ++i)
            {
                auto name = std::string(SDL_GetAudioDeviceName(sdl_audio_recording_device_ids[i]));
                devices.push_back(CaptureDevice(i, name));
            }
        }
        return devices;
    }
};

inline bool operator==(const CaptureDevice& lhs, const CaptureDevice& rhs){
    return lhs.id == rhs.id;
}

namespace std
{
    template<>
    class hash<CaptureDevice>
    {
    public:
        std::uint64_t operator()(const CaptureDevice& device)const
        {
            // compute hash of capture device using a variant of
            // the Fowler-Noll-Vo hash function
            // see https://en.cppreference.com/w/cpp/utility/hash/operator()
            constexpr std::uint64_t prime{0x100000001B3};
            std::uint64_t result{0xcbf29ce484222325};
            for (std::uint64_t i{}; i < sizeof(device.id); ++i)
            {
                uint64_t mask = 255 << i * 8; // select each byte of the input id
                result = (result * prime) ^ ((device.id * mask) >> i * 8); // xor it with the compounding hash result.
            }
            return result;
        }
    };
}
