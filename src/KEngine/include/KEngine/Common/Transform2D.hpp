#pragma once

#include <cstdint>

namespace ke
{

    using Angle = float;

    class Transform2D
    {
    public:
        using PointType = std::int32_t;

        PointType x = 0;
        PointType y = 0;
        Angle angle = 0.0f;

        Transform2D() = default;
        Transform2D(PointType p_x, PointType p_y, Angle p_angle) : x(p_x), y(p_y), angle(p_angle) {}

        inline Transform2D & operator+=(const Transform2D & transform)
        {
            this->x     += transform.x;
            this->y     += transform.y;
            this->angle += transform.angle;
            return *this;
        }

        inline Transform2D & operator-=(const Transform2D & transform)
        {
            this->x     -= transform.x;
            this->y     -= transform.y;
            this->angle -= transform.angle;
            return *this;
        }
    };

    inline ke::Transform2D operator+(const Transform2D & lhs, const Transform2D & rhs)
    {
        return{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.angle + rhs.angle };
    }

    inline ke::Transform2D operator-(const Transform2D & lhs, const Transform2D & rhs)
    {
        return{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.angle - rhs.angle };
    }

}