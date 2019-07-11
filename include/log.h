#pragma once

#include <iostream>

#define LOG(level)                                  \
    if (level <= platform::default_log.get_level()) \
    platform::default_log.get_stream()              \
        << '\n'                                     \
        << '[' << __FILE__ << ":" << std::dec << __LINE__ << "] "

namespace platform
{
    class log
    {
        log(log const &) = delete;
        log(log &&)      = delete;
        log & operator=(log const &) = delete;
        log & operator=(log &&) = delete;

    public:
        enum log_level
        {
            None,
            Error,
            Info,
            Verbose
        };

        log() = default;

        void set_stream(std::ostream &);
        void set_level(log_level);

        std::ostream & get_stream();
        log_level      get_level();

    private:
        log_level      level  = Info;
        std::ostream * stream = &std::cout;
    };

    extern log default_log;
}
