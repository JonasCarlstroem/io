#pragma once
#include "io_reader.hpp"
#include <functional>
#include <mutex>
#include <thread>

namespace io {

class async_reader : public reader {
  public:
    using callback_t = std::function<void(const std::string &)>;

    async_reader(reader &target, callback_t callback, size_t read_chunk = 4096)
        : target_(target), callback_(callback), read_chunk_(read_chunk),
          thread_(&async_reader::worker, this) {}

    ~async_reader() { stop(); }

    std::string read(size_t max_bytes = 4096) override {
        return target_.read(max_bytes);
    }

    std::string read_line() override { return target_.read_line(); }

    void stop() {
        {
            std::lock_guard lock(mutex_);
            stopped_ = true;
        }
        if (thread_.joinable())
            thread_.join();
    }

  private:
    void worker() {
        while (!stopped_) {
            try {
                auto data = target_.read(read_chunk_);
                if (data.empty())
                    break;
                callback_(data);
            } catch (const std::exception &e) {
                break;
            }
        }
    }

    reader &target_;
    callback_t callback_;
    size_t read_chunk_;
    std::mutex mutex_;
    bool stopped_ = false;
    std::thread thread_;
};

} // namespace io