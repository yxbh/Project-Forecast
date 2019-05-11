#pragma once

#pragma warning(push)
#pragma warning(disable:4244)  // conversion from 'const unisgned __int64' to 'int', possible loss of data.
#pragma warning(disable:6387)
#pragma warning(disable:26110)  // Caller failing to hold lock
#pragma warning(disable:26439)  // This kidn of funciton may not throw. Declare it 'noexcept' (f.6).
#pragma warning(disable:26495)  // Variable is uninitialized. ALways initialize a member variable (type.6)
#pragma warning(disable:26498)
#pragma warning(disable:26451)  // Arithmetic overflow
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

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
