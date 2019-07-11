#include "log.h"

namespace platform
{
    void log::set_stream(std::ostream & ref)
    {
        stream = &ref;
    }
    void log::set_level(log::log_level value)
    {
        level = value;
    }
    std::ostream & log::get_stream()
    {
        return *stream;
    }
    log::log_level log::get_level()
    {
        return level;
    }

    log default_log;
}
