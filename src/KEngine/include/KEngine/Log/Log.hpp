#pragma once

#include <spdlog/spdlog.h>

namespace ke
{
    using namespace spdlog; // alias the spdlog namespace to make it our own.

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
