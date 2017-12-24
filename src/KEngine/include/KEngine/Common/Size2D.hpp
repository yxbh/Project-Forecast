#pragma once

#include <cstdint>

namespace ke
{

    template <typename ValueType>
    struct Size2D
    {
        ValueType width = 0;
        ValueType height = 0;
    };

    using Size2DInt32  = Size2D<std::int32_t>;
    using Size2DInt64  = Size2D<std::int64_t>;
    using Size2DUInt32 = Size2D<std::uint32_t>;
    using Size2DUInt64 = Size2D<std::uint64_t>;
    using Size2DFloat  = Size2D<float>;
    using Size2DDouble = Size2D<double>;

}