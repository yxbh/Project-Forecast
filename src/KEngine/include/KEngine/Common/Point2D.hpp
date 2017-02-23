#pragma once

#include <cstdint>

namespace ke
{

    struct Point2D
    {
        using ValueType = std::int32_t;

        ValueType x = 0;
        ValueType y = 0;

        Point2D(ValueType p_x = 0, ValueType p_y = 0) : x(p_x), y(p_y) {}
    };

}