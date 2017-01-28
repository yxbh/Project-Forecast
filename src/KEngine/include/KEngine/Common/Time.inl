#pragma once

namespace ke
{

inline Time::Time(const Value p_Value)
    : m_TimeInMicroseconds(0)
{
    KE_UNUSED(p_Value);
}

inline double Time::asSeconds(void) const
{
    return static_cast<double>(m_TimeInMicroseconds)/1000000.0;
}

inline std::int32_t Time::asMilliseconds(void) const
{
    return static_cast<std::int32_t>(m_TimeInMicroseconds/1000);
}

inline std::int64_t Time::asMicroseconds(void) const
{
    return m_TimeInMicroseconds;
}

inline std::int64_t Time::count(void) const
{
    return m_TimeInMicroseconds;
}

inline std::int64_t Time::getCount(void) const
{
    return count();
}


inline ke::Time Time::seconds(const double p_Amount)
{
    return ke::Time(static_cast<std::int64_t>(p_Amount * 1000000));
}

inline ke::Time Time::milliseconds(const std::int64_t p_Amount)
{
    return ke::Time(static_cast<std::int64_t>(p_Amount*1000));
}

inline ke::Time Time::microseconds(const std::int64_t p_Amount)
{
    return ke::Time(p_Amount);
}

inline Time::Time(const std::int64_t p_Microseconds)
    : m_TimeInMicroseconds(p_Microseconds) {}

inline bool operator==(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asMicroseconds() == p_Right.asMicroseconds();
}

inline bool operator!=(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asMicroseconds() != p_Right.asMicroseconds();
}

inline bool operator<(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asMicroseconds() < p_Right.asMicroseconds();
}

inline bool operator>(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asMicroseconds() > p_Right.asMicroseconds();
}

inline bool operator<=(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asMicroseconds() <= p_Right.asMicroseconds();
}

inline bool operator>=(const ke::Time p_Left, const ke::Time p_Right)
{
    return p_Left.asMicroseconds() >= p_Right.asMicroseconds();
}

inline ke::Time operator-(const ke::Time p_Time)
{
    return ke::Time::microseconds(-1 * p_Time.asMicroseconds());
}

inline ke::Time operator+(const ke::Time p_Left, const ke::Time p_Right)
{
    return ke::Time::microseconds(p_Left.asMicroseconds()+p_Right.asMicroseconds());
}

inline ke::Time & operator+=(ke::Time & p_Left, const ke::Time p_Right)
{
    return p_Left = p_Left + p_Right;
}

inline ke::Time operator-(const ke::Time p_Left, const ke::Time p_Right)
{
    return ke::Time::microseconds(p_Left.asMicroseconds()-p_Right.asMicroseconds());
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
    return ke::Time::microseconds(p_Left.asMicroseconds()*p_Right);
}

inline ke::Time operator*(const double p_Left, const ke::Time p_Right)
{
    return ke::Time::seconds(p_Right.asSeconds()*p_Left);
}

inline ke::Time operator*(const std::int64_t p_Left, const ke::Time p_Right)
{
    return ke::Time::microseconds(p_Right.asMicroseconds()*p_Left);
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
    return ke::Time::microseconds(p_Left.asMicroseconds()*p_Right);
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
    return ke::Time::microseconds((p_rLeft.asMicroseconds() % p_rRight.asMicroseconds()));
}

} // KE ns
