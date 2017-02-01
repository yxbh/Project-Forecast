#pragma once

#include "macros.hpp"

#include <cstdint>

namespace ke
{

    /// <summary>
    /// A class which defines a period of time (i.e. a moment of time). The value can be negative.
    /// </summary>
    /// 
    /// The Time can be evalued to abirtary precision (seconds(double), milliseconds(in32) and microseconds(int64).
    /// 
    /// The as*() methods may lose precision when returning its value.
    /// E.g. a Time object representing 1500 microseconds will return 1 when asMilliseconds() is called.
    class Time
    {
    public:
        enum class Value
        {
            Zero
        };
        static const Time Zero;

    private:
        std::int64_t m_TimeInMicroseconds = 0;

    public:
        Time(void) = default;
        Time(const Value p_Value);

        double asSeconds(void) const;
        std::int32_t asMilliseconds(void) const;
        std::int64_t asMicroseconds(void) const;

        std::int64_t count(void) const;
        std::int64_t getCount(void) const;

        static ke::Time seconds(const double p_Amount);
        static ke::Time milliseconds(const std::int64_t p_Amount);
        static ke::Time microseconds(const std::int64_t p_Amount);

    private:
        explicit Time(const std::int64_t p_Microseconds);

    };

    bool operator==(const ke::Time p_Left, const ke::Time p_Right);
    bool operator!=(const ke::Time p_Left, const ke::Time p_Right);
    bool operator<(const ke::Time p_Left, const ke::Time p_Right);
    bool operator>(const ke::Time p_Left, const ke::Time p_Right);
    bool operator<=(const ke::Time p_Left, const ke::Time p_Right);
    bool operator>=(const ke::Time p_Left, const ke::Time p_Right);
    ke::Time operator-(const ke::Time p_Time);
    ke::Time operator+(const ke::Time p_Left, const ke::Time p_Right);
    ke::Time & operator+=(ke::Time & p_Left, const ke::Time p_Right);
    ke::Time operator-(const ke::Time p_Left, const ke::Time p_Right);
    ke::Time & operator-=(ke::Time & p_Left, const ke::Time p_Right);
    ke::Time operator*(const ke::Time p_Left, const double p_Right);
    ke::Time operator*(const ke::Time p_Left, const std::int64_t p_Right);
    ke::Time operator*(const double p_Left, const ke::Time p_Right);
    ke::Time operator*(const std::int64_t p_Left, const ke::Time p_Right);
    ke::Time & operator*=(ke::Time & p_Left, const double p_Right);
    ke::Time & operator*=(ke::Time & p_Left, const std::int64_t p_Right);
    double operator/(ke::Time p_Left, const ke::Time p_Right);
    ke::Time operator/(const ke::Time p_Left, const double p_Right);
    ke::Time operator/(const ke::Time p_Left, const std::int64_t p_Right);
    ke::Time & operator/=(ke::Time & p_rLeft, const double p_Right);
    ke::Time & operator/=(ke::Time & p_rLeft, const std::int64_t p_Right);
    ke::Time operator%(const ke::Time & p_rLeft, const ke::Time & p_rRight);

} // KE ns

#include "Time.inl"
