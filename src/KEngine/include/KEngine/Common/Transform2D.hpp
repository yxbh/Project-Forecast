#pragma once

#include <cstdint>

namespace ke
{

    using Angle = float;

    class Transform2D
    {
    public:
        using PointType = double;

        PointType x = 0;
        PointType y = 0;
        Angle rotation = 0.0f;
        float scaleX = 1.0f;
        float scaleY = 1.0f;

        Transform2D() = default;
        Transform2D(PointType p_x, PointType p_y, Angle p_angle = 0.0f, float p_scaleX = 1.0f, float p_scaleY = 1.0f)
            : x(p_x), y(p_y), rotation(p_angle), scaleX(p_scaleX), scaleY(p_scaleY) {}

        inline Transform2D & operator+=(const Transform2D & transform)
        {
            this->x        += transform.x;
            this->y        += transform.y;
            this->rotation += transform.rotation;
            this->scaleX   *= transform.scaleX;
            this->scaleY   *= transform.scaleY;
            return *this;
        }

        inline Transform2D & operator-=(const Transform2D & transform)
        {
            this->x        -= transform.x;
            this->y        -= transform.y;
            this->rotation -= transform.rotation;
            this->scaleX   /= transform.scaleX;
            this->scaleY   /= transform.scaleY;
            return *this;
        }
    };

    inline ke::Transform2D operator+(const Transform2D & lhs, const Transform2D & rhs)
    {
        return
        {
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.rotation + rhs.rotation,
            lhs.scaleX * rhs.scaleX,
            lhs.scaleY * rhs.scaleY
        };
    }

    inline ke::Transform2D operator-(const Transform2D & lhs, const Transform2D & rhs)
    {
        return
        {
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.rotation - rhs.rotation,
            lhs.scaleX / rhs.scaleX,
            lhs.scaleY / rhs.scaleY
        };
    }

}