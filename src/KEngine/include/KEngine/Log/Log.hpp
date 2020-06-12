#pragma once

#ifndef NOGDI  // The spdlog headers can include wingdi.h which can cause macro definition issues for us.
#define NOGDI  // we specify no GDI here.
#endif

#pragma warning(push)
#pragma warning(disable:4244)  // conversion from 'const unisgned __int64' to 'int', possible loss of data.
#pragma warning(disable:6011)  // Dereferencing NULL pointer
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

#include <mutex>


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

#ifdef RUN_UNIT_TESTS
        using log_factory = ke::log::default_factory;
#else
        using log_factory = ke::log::async_factory;
#endif // RUN_UNIT_TESTS


        /// <summary>
        /// Obtain the default logger object.
        /// </summary>
        /// <returns>The logger instance.</returns>
        inline static auto instance() -> auto
        {
            static auto logger = ke::log::stdout_color_mt<log_factory>("default"); // logger object is only constructed when this function is called.
            //return ke::log::get("General");
            return logger;
        }

        /// <summary>
        /// Create an unshared logger object with the given name and default settings.
        /// </summary>
        /// <param name="p_loggerName"></param>
        /// <returns></returns>
        inline static auto createDefaultLogger(char const* const p_loggerName) -> auto
        {
            assert(p_loggerName);
            auto sink = std::make_shared<ke::log::sinks::stdout_color_sink_mt>();

#ifdef RUN_UNIT_TESTS
            auto logger = std::make_shared<ke::log::logger>(p_loggerName, sink);
#else
            // initialise the default spdlog thread pool if it hasn't been created.
            static bool threadpool_init = false;
            static std::mutex tp_lock;
            if (!threadpool_init)
            {
                std::scoped_lock<std::mutex> lock(tp_lock);
                if (!threadpool_init)
                {
                    ke::log::init_thread_pool(8192, 1);  // default values from spdlog doc.
                    threadpool_init = true;
                }
            }

            auto logger = std::make_shared<ke::log::async_logger>(p_loggerName, sink, ke::log::thread_pool());
#endif // RUN_UNIT_TESTS

            return logger;
        }

        inline static auto instance(char const* const p_loggerName) -> auto
        {
            if (p_loggerName == std::string("default"))
            {
                return instance();
            }
            return ke::log::get(p_loggerName);
        }
    };
}
