#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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
        /// Obtain the logger object.
        /// </summary>
        /// <returns>The logger instance.</returns>
        static auto instance() -> auto
        {
#ifdef RUN_UNIT_TESTS
            using log_factory = ke::log::default_factory;
#else
            using log_factory = ke::log::async_factory;
#endif // RUN_UNIT_TESTS
            static auto logger = ke::log::stdout_color_mt<log_factory>("General"); // logger object is only constructed when this function is called.
            static auto loggerQuickPtr = logger.get();
            return loggerQuickPtr;
        }
    };
}
