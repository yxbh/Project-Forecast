#pragma once

#include <cstdint>

namespace ke
{

    template <typename ValueType>
    struct Point2D
    {
        ValueType x = 0;
        ValueType y = 0;
    };

    using Point2DInt32  = Point2D<std::int32_t>;
    using Point2DInt64  = Point2D<std::int64_t>;
    using Point2DUInt32 = Point2D<std::uint32_t>;
    using Point2DUInt64 = Point2D<std::uint64_t>;
    using Point2DFloat  = Point2D<float>;
    using Point2DDouble = Point2D<double>;

}