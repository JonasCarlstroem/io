#pragma once
#include "writer.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace io {

class async_writer : public writer {
  public:
    explicit async_writer(writer &target)
        : target_(target), thread_(&async_writer::worker, this) {}

    ~async_writer() { stop(); }

    void write(const std::string &data) override {
        {
            std::lock_guard lock(mutex_);
            queue_.push(data);
        }
        cv_.notify_one();
    }

    void flush() {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [&] { return queue_.empty(); });
    }

    void stop() {
        {
            std::lock_guard lock(mutex_);
            stopped_ = true;
        }
        cv_.notify_one();
        if (thread_.joinable())
            thread_.join();
    }

  private:
    void worker() {
        while (true) {
            std::string data;
            {
                std::unique_lock lock(mutex_);
                cv_.wait(lock, [&] { return stopped_ || !queue_.empty(); });
                if (stopped_ && queue_.empty())
                    break;
                data = std::move(queue_.front());
                queue_.pop();
            }
            target_.write(data);
        }
    }

    writer &target_;
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopped_ = false;
    std::thread thread_;
};

} // namespace io