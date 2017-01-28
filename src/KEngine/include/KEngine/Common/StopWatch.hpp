#pragma once

#include "Time.hpp"

#include <chrono>
#include <type_traits>

namespace ke
{

    /**
     * \class StopWatch
     * \brief
     */
    class StopWatch
    {
    public:
        using DurationType          = ke::Time;
        using DurationInternalType  = std::chrono::milliseconds;
        using ClockType             = std::chrono::high_resolution_clock;
        using TimePointType         = ClockType::time_point;

    private:
        DurationInternalType    m_Duration;
        TimePointType           m_BeforeTP, m_AfterTP;

    public:
        StopWatch(void)
            : m_Duration(0), m_BeforeTP(ClockType::now()), m_AfterTP(m_BeforeTP) {}

        /** \return the elapsed time as the templated duration type. */
        inline DurationType elapsed(void)
        {
            m_AfterTP = ClockType::now();
            m_Duration += std::chrono::duration_cast<DurationInternalType>(m_AfterTP - m_BeforeTP);
            m_BeforeTP = m_AfterTP;
            return ke::Time::milliseconds(m_Duration.count());
        }

        inline DurationType getElapsed(void)
        {
            return elapsed();
        }

        inline void restart(void)
        {
            m_BeforeTP = ClockType::now();
            m_Duration = DurationInternalType(0);
        }
    }; // ElapsedTimer class

}