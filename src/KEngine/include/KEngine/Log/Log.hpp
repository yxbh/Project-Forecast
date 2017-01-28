#pragma once

#include <spdlog\spdlog.h>

namespace ke = spdlog; // alias the spdlog namespace to make it our own.

namespace ke
{
    class Log
    {
    public:

        static auto instance() -> auto
        {
            static auto logger = ke::stdout_color_mt("console");
            return logger.get();
        }
    };
}
