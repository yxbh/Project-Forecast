#pragma once

#include <cstdint>

namespace ke
{

    template <typename ValueType>
    struct Dimension2D
    {
        ValueType weight = 0;
        ValueType height = 0;
    };

    using Dimension2DInt32  = Dimension2D<std::int32_t>;
    using Dimension2DInt64  = Dimension2D<std::int64_t>;
    using Dimension2DUInt32 = Dimension2D<std::uint32_t>;
    using Dimension2DUInt64 = Dimension2D<std::uint64_t>;
    using Dimension2DFloat  = Dimension2D<float>;
    using Dimension2DDouble = Dimension2D<double>;

}
