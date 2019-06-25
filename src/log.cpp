#include "log.h"

namespace platform
{
  void log::set_stream(std::ostream& ref)
  {
    this->stream = &ref;
  }
  void log::set_level(log::log_level value)
  {
    this->level = value;
  }
  std::ostream& log::get_stream()
  {
    return *this->stream;
  }
  log::log_level log::get_level()
  {
    return this->level;
  }

  log default_log;
}
