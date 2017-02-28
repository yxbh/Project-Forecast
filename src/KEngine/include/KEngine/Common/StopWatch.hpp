#pragma once

#include "Time.hpp"

#include <chrono>

namespace ke
{

    /// <summary>
    /// A class that measures time elapsed from a previous point in time.
    /// </summary>
    class StopWatch
    {
    public:
        using DurationType          = ke::Time;
        using DurationInternalType  = std::chrono::nanoseconds;
        using ClockType             = std::chrono::high_resolution_clock;
        using TimePointType         = ClockType::time_point;

    private:
        TimePointType m_TimePoint;

    public:
        /// <summary>
        /// Construct a stop watch and begin the timer.
        /// </summary>
        StopWatch(void)
            : m_TimePoint(ClockType::now()) {}

        /// <summary>
        /// Get the elapsed time.
        /// </summary>
        /// <returns>The elapsed time.</returns>
        inline DurationType elapsed(void)
        {
            const auto elapsed = std::chrono::duration_cast<DurationInternalType>(ClockType::now() - m_TimePoint);
            return ke::Time::nanoseconds(elapsed.count());
        }

        /// <summary>
        /// Get the elapsed time since the last time the stop watch was restarted.
        /// </summary>
        /// <returns>The elapsed time.</returns>
        inline DurationType getElapsed(void)
        {
            return elapsed();
        }

        /// <summary>
        /// Restart the stopwatch. Next time elapsed() is called it will be measured from this point in time.
        /// </summary>
        inline void restart(void)
        {
            m_TimePoint = ClockType::now();
        }
    }; // ElapsedTimer class

}
