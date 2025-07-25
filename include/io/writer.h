#pragma once
#include <string>

namespace io {

class writer {
  public:
    virtual ~writer()                           = default;
    virtual void write(const std::string &data) = 0;
    virtual void flush()                        = 0;
};

} // namespace io