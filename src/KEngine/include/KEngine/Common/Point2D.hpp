#pragma once

#include <cstdint>

namespace ke
{

    template <typename ValueType>
    struct Point2D
    {
        ValueType x = 0;
        ValueType y = 0;

        Point2D<ValueType> & operator+=(const Point2D<ValueType> & p_point2d);
        Point2D<ValueType> & operator+=(Point2D<ValueType> && p_point2d);
        Point2D<ValueType> & operator-=(const Point2D<ValueType> & p_point2d);
        Point2D<ValueType> & operator-=(Point2D<ValueType> && p_point2d);

        Point2D<ValueType> operator+(const Point2D<ValueType> & p_point2d);
        Point2D<ValueType> operator-(const Point2D<ValueType> & p_point2d);
        Point2D<ValueType> operator+(Point2D<ValueType> && p_point2d);
        Point2D<ValueType> operator-(Point2D<ValueType> && p_point2d);
    };

    using Point2DInt32  = Point2D<std::int32_t>;
    using Point2DInt64  = Point2D<std::int64_t>;
    using Point2DUInt32 = Point2D<std::uint32_t>;
    using Point2DUInt64 = Point2D<std::uint64_t>;
    using Point2DFloat  = Point2D<float>;
    using Point2DDouble = Point2D<double>;

    template <typename ValueType>
    inline Point2D<ValueType> & Point2D<ValueType>::operator+=(const Point2D<ValueType> & p_point2d)
    {
        this->x += p_point2d.x;
        this->y += p_point2d.y;
        return *this;
    }

    template <typename ValueType>
    inline Point2D<ValueType> & Point2D<ValueType>::operator+=(Point2D<ValueType> && p_point2d)
    {
        this->x += p_point2d.x;
        this->y += p_point2d.y;
        return *this;
    }

    template <typename ValueType>
    inline Point2D<ValueType> & Point2D<ValueType>::operator-=(const Point2D<ValueType> & p_point2d)
    {
        this->x -= p_point2d.x;
        this->y -= p_point2d.y;
        return *this;
    }

    template <typename ValueType>
    inline Point2D<ValueType> & Point2D<ValueType>::operator-=(Point2D<ValueType> && p_point2d)
    {
        this->x -= p_point2d.x;
        this->y -= p_point2d.y;
        return *this;
    }

    template <typename ValueType>
    inline Point2D<ValueType> Point2D<ValueType>::operator+(const Point2D<ValueType> & p_point2d)
    {
        return { this->x + p_point2d.x, this->y + p_point2d.y };
    }

    template <typename ValueType>
    inline Point2D<ValueType> Point2D<ValueType>::operator-(const Point2D<ValueType> & p_point2d)
    {
        return { this->x - p_point2d.x, this->y - p_point2d.y };
    }

    template <typename ValueType>
    inline Point2D<ValueType> Point2D<ValueType>::operator+(Point2D<ValueType> && p_point2d)
    {
        return { this->x + p_point2d.x, this->y + p_point2d.y };
    }

    template <typename ValueType>
    inline Point2D<ValueType> Point2D<ValueType>::operator-(Point2D<ValueType> && p_point2d)
    {
        return { this->x - p_point2d.x, this->y - p_point2d.y };
    }
}