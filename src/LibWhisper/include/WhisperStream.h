#pragma once

#include <LibWhisper.h>

#include <cassert>
#include <cstdint>
#include <chrono>
#include <ctime>

#include <memory>
#include <string>
#include <vector>
#include <thread>

#include <CaptureDevice.h>
#include <stream.h>


struct Segment {
    std::string text;
    uint64_t t0;
    uint64_t t1;
};

typedef std::vector<Segment> OrderedSegments;

namespace std{
    std::string to_string(OrderedSegments segments);
}

class WhisperStream {
public:
    
    WhisperStream (std::string model, std::shared_ptr<CaptureDevice> device = nullptr, std::chrono::seconds window = static_cast<std::chrono::seconds>(300));
    
    void task(std::stop_token stoken);
    
    int callback( std::string text, int64_t t0, int64_t t1, WhisperStream *ctx);

    OrderedSegments& getSegments(){
        return segments;
    }
protected:
    std::jthread waiter;
    OrderedSegments segments;
    bool alive = true;
    
    //TODO: replace a string with a URI representation, ideally something accepted into or leveraging the C++ standard library
    std::string model;
    std::shared_ptr<CaptureDevice> device;
    std::chrono::seconds window;
};
