#pragma once

#include <cstdint>

namespace ke
{

    template <typename ValueType>
    struct Rect2D
    {
        ValueType bottom = 0;
        ValueType left = 0;
        ValueType width = 0;
        ValueType height = 0;
    };

    using Rect2DInt32  = Rect2D<std::int32_t>;
    using Rect2DInt64  = Rect2D<std::int64_t>;
    using Rect2DUInt32 = Rect2D<std::uint32_t>;
    using Rect2DUInt64 = Rect2D<std::uint64_t>;
    using Rect2DFloat  = Rect2D<float>;
    using Rect2DDouble = Rect2D<double>;

}