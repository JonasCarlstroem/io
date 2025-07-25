#pragma once
#include <string>

namespace io {

class reader {
  public:
    virtual ~reader()                                 = default;
    virtual std::string read(size_t max_bytes = 4096) = 0;
    virtual std::string read_line()                   = 0;
};

} // namespace io