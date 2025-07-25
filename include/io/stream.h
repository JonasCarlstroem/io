#pragma once
#include "reader.h"
#include "writer.h"

namespace io {

class stream {
    HANDLE read_handle_  = INVALID_HANDLE_VALUE;
    HANDLE write_handle_ = INVALID_HANDLE_VALUE;

  protected:
    std::unique_ptr<reader> r_;
    std::unique_ptr<writer> w_;

  public:
    stream(std::unique_ptr<reader> r, std::unique_ptr<writer> w)
        : r_(std::move(r)), w_(std::move(w)) {}

    stream(const stream &)            = delete;
    stream &operator=(const stream &) = delete;

    stream(stream &&other) noexcept
        : r_(std::move(other.r_)), w_(std::move(other.w_)) {}

    stream &operator=(stream &&other) noexcept {
        if (this != &other) {
            r_ = std::move(other.r_);
            w_ = std::move(other.w_);
        }
        return *this;
    }

    std::string read(size_t max_bytes = 4096) { return r_->read(max_bytes); }

    std::string read_line() { return r_->read_line(); }

    void write(const std::string &data) { w_->write(data); }

    reader &get_reader() { return *r_; }
    writer &get_writer() { return *w_; }

  private:
    /*reader& reader_;
    writer& writer_;*/
};

} // namespace io