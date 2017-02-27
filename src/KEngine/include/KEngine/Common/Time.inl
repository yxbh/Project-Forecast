#pragma once

namespace ke
{

inline Time::Time(const Value p_Value)
    : m_TimeInNanoseconds(0)
{
    KE_UNUSED(p_Value);
}

inline double Time::asSeconds(void) const
{
    return static_cast<double>(m_TimeInNanoseconds)/1000000000.0;
}

inline std::int32_t Time::asMilliseconds(void) const
{
    return static_cast<std::int32_t>(m_TimeInNanoseconds /1000000);
}

inline std::int64_t Time::asMicroseconds(void) const
{
    return m_TimeInNanoseconds / 1000;
}

inline std::int64_t Time::asNanoseconds(void) const
{
    return m_TimeInNanoseconds;
}

inline std::int64_t Time::count(void) const
{
    return m_TimeInNanoseconds;
}

inline std::int64_t Time::getCount(void) const
{
    return count();
}


inline ke::Time Time::seconds(const double p_Amount)
{
    return ke::Time(static_cast<std::int64_t>(p_Amount * 1000000000));
}

inline ke::Time Time::milliseconds(const std::int64_t p_Amount)
{
    return ke::Time(static_cast<std::int64_t>(p_Amount*1000000));
}

inline ke::Time Time::microseconds(const std::int64_t p_Amount)
{
    return ke::Time(p_Amount * 1000);
}

inline ke::Time Time::nanoseconds(const std::int64_t p_Amount)
{
    return ke::Time(p_Amount);
}

inline Time::Time(const std::int64_t p_Nanoseconds)
    : m_TimeInNanoseconds(p_Nanoseconds) {}

inline bool operator==(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asNanoseconds() == p_Right.asNanoseconds();
}

inline bool operator!=(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asNanoseconds() != p_Right.asNanoseconds();
}

inline bool operator<(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asNanoseconds() < p_Right.asNanoseconds();
}

inline bool operator>(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asNanoseconds() > p_Right.asNanoseconds();
}

inline bool operator<=(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asNanoseconds() <= p_Right.asNanoseconds();
}

inline bool operator>=(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asNanoseconds() >= p_Right.asNanoseconds();
}

inline ke::Time operator-(const ke::Time p_Time)
{
    return ke::Time::nanoseconds(-1 * p_Time.asNanoseconds());
}

inline ke::Time operator+(const ke::Time p_Left, const ke::Time p_Right)
{
    return ke::Time::nanoseconds(p_Left.asNanoseconds()+p_Right.asNanoseconds());
}

inline ke::Time & operator+=(ke::Time & p_Left, const ke::Time p_Right)
{
    return p_Left = p_Left + p_Right;
}

inline ke::Time operator-(const ke::Time p_Left, const ke::Time p_Right)
{
    return ke::Time::nanoseconds(p_Left.asNanoseconds()-p_Right.asNanoseconds());
}

inline ke::Time & operator-=(ke::Time & p_Left, const ke::Time p_Right)
{
    return p_Left = p_Left - p_Right;
}

inline ke::Time operator*(const ke::Time p_Left, const double p_Right)
{
    return ke::Time::seconds(p_Left.asSeconds()*p_Right);
}

inline ke::Time operator*(const ke::Time p_Left, const std::int64_t p_Right)
{
    return ke::Time::nanoseconds(p_Left.asNanoseconds()*p_Right);
}

inline ke::Time operator*(const double p_Left, const ke::Time p_Right)
{
    return ke::Time::seconds(p_Right.asSeconds()*p_Left);
}

inline ke::Time operator*(const std::int64_t p_Left, const ke::Time p_Right)
{
    return ke::Time::nanoseconds(p_Right.asNanoseconds()*p_Left);
}

inline ke::Time & operator*=(ke::Time & p_Left, const double p_Right)
{
    return p_Left = p_Left * p_Right;
}

inline ke::Time & operator*=(ke::Time & p_Left, const std::int64_t p_Right)
{
    return p_Left = p_Left * p_Right;
}

inline double operator/(ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asSeconds()/p_Right.asSeconds();
}

inline ke::Time operator/(const ke::Time p_Left, const double p_Right)
{
    return ke::Time::seconds(p_Left.asSeconds()/p_Right);
}

inline ke::Time operator/(const ke::Time p_Left, const std::int64_t p_Right)
{
    return ke::Time::nanoseconds(p_Left.asNanoseconds()*p_Right);
}

inline ke::Time & operator/=(ke::Time & p_rLeft, const double p_Right)
{
    return p_rLeft = p_rLeft / p_Right;
}

inline ke::Time & operator/=(ke::Time & p_rLeft, const std::int64_t p_Right)
{
    return p_rLeft = p_rLeft / p_Right;
}

inline ke::Time operator%(const ke::Time & p_rLeft, const ke::Time & p_rRight)
{
    return ke::Time::nanoseconds((p_rLeft.asNanoseconds() % p_rRight.asNanoseconds()));
}

} // KE ns
