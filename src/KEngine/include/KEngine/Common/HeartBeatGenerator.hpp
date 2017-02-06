#pragma once

#include "KEngine/Common/StopWatch.hpp"
#include "KEngine/Common/Time.hpp"

namespace ke
{
    
    /// <summary>
    /// A timer class that produces a heart beat (when evaluated) whenever the specified time span has passed.
    /// </summary>
    class HeartBeatGenerator
    {
    public:
        inline HeartBeatGenerator(const ke::Time & heartBeatRate) : heartBeatRate(heartBeatRate) {}

        /// <summary>
        /// Check to see if a heart beat is available.
        /// </summary>
        /// <returns>true if there's a heartbeat.</returns>
        inline operator bool()
        {
            elapsedTime += timer.getElapsed();
            timer.restart();
            if (elapsedTime >= heartBeatRate)
            {
                elapsedTime -= heartBeatRate;
                return true;
            }

            return false;
        }

    private:
        ke::StopWatch timer;
        ke::Time elapsedTime;
        ke::Time heartBeatRate = ke::Time::Zero; /// time span between heart beats.

    };

}