#pragma once
#include <Windows.h>
#include <stdexcept>
#include <streambuf>
#include <vector>
#include <winapi/handle.h>

namespace io {

// using win_handle = winapi::handle;

class h_streambuf : public std::streambuf {
    static constexpr size_t buffer_size = 4096;
    HANDLE handle_;
    std::vector<char> buffer_;

  public:
    explicit h_streambuf(win_handle handle) : h_streambuf(handle.get()) {}

    explicit h_streambuf(HANDLE h) : handle_(h), buffer_(buffer_size) {
        setp(buffer_.data(), buffer_.data() + buffer_.size() - 1);
    }

  protected:
    int_type overflow(int_type ch) override {
        if (ch != traits_type::eof()) {
            *pptr() = ch;
            pbump(1);
        }

        if (flush_buffer() == -1)
            return traits_type::eof();
        return ch;
    }

    int_type underflow() override {
        if (gptr() < egptr())
            return traits_type::to_int_type(*gptr());

        DWORD bytes_read = 0;
        BOOL success     = ReadFile(
            handle_, buffer_.data(), static_cast<DWORD>(buffer_.size()),
            &bytes_read, nullptr
        );
        if (!success || bytes_read == 0)
            return traits_type::eof();

        setg(buffer_.data(), buffer_.data(), buffer_.data() + bytes_read);
        return traits_type::to_int_type(*gptr());
    }

    int sync() override { return flush_buffer() == -1 ? -1 : 0; }

  private:
    int flush_buffer() {
        ptrdiff_t n = pptr() - pbase();
        if (n == 0)
            return 0;
        DWORD written = 0;
        if (!WriteFile(
                handle_, pbase(), static_cast<DWORD>(n), &written, nullptr
            )) {
            return -1;
        }
        pbump(-static_cast<int>(n));
        return 0;
    }
};

} // namespace io