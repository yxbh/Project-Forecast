#pragma once

#include <cstdint>

namespace ke
{

    struct Color
    {
    public:
        static constexpr uint8_t DEFAULT_VALUE = 255u;
        std::uint8_t r = DEFAULT_VALUE;
        std::uint8_t g = DEFAULT_VALUE;
        std::uint8_t b = DEFAULT_VALUE;
        std::uint8_t a = DEFAULT_VALUE;


        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color TRANSPARENT;
    };

    using Colour = Color;

}
