#include <WhisperStream.h>
#include <functional>

std::string to_string(OrderedSegments segments) {
    std::string result;
    for (const auto &segment : segments) {
        result += segment.text;
    }
    return result;
}

WhisperStream::WhisperStream (std::string model, std::shared_ptr<CaptureDevice> device, std::chrono::seconds window) {
    this->model = model;
    this->device = device;
    this->window = window;

    waiter = std::jthread(std::bind(&WhisperStream::task, this, std::placeholders::_1));
}

void WhisperStream::task(std::stop_token stoken) {
    auto params = stream_default_params();
    params.model = model.c_str();

    if (device != nullptr) {
        params.capture_id = device->id;
    }

    auto ctx = stream_init(params);
    if (ctx == nullptr) {
        return;
    }

    auto callbackFn = +[](const char *text, int64_t t0, int64_t t1, void *ctx) -> int {
        return static_cast<WhisperStream*>(ctx)->callback(std::string(text), t0, t1, static_cast<WhisperStream *>(ctx));
    };

    while (!stoken.stop_requested()) {
        auto errVal = stream_run(ctx, this, callbackFn);

        if (errVal != 0) {
            break;
        }
    }

    stream_free(ctx);
    alive = false;
}

int WhisperStream::callback( std::string text, int64_t t0, int64_t t1, WhisperStream *ctx) {
    if (segments.empty() || text.empty()) {
        segments.push_back(Segment("", -1, -1));
    }

    segments[segments.size() - 1] = Segment(text, t0, t1);

    size_t k = 0;
    for (auto segment : segments) {
        auto last = segments.back();
        if (std::chrono::milliseconds(last.t0 - segment.t0) > window) {
            k += 1;
        }
    }

    segments.erase(segments.begin(), segments.begin() + k);
    return 0;
}
