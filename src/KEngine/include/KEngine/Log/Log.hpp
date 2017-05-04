#pragma once

#include <spdlog/spdlog.h>

/// <summary>
/// Reference: https://github.com/gabime/spdlog/wiki
/// </summary>

namespace ke { namespace log
{
    using namespace spdlog; // alias the spdlog namespace to make it our own.
}}

namespace ke
{

    class Log
    {
    public:
        /// <summary>
        /// Obtain the logger object. Do not call this before calling ke::App::exec().
        /// </summary>
        /// <returns>The logger instance.</returns>
        static auto instance() -> auto
        {
            static auto logger = ke::log::stdout_color_mt("General"); // logger object is only constructed when this function is called.
            return logger.get();
        }
    };
}
