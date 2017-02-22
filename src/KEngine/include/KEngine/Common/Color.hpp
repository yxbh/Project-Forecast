#pragma once

#include <cstdint>

namespace ke
{

    struct Color
    {
    public:
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;


        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color TRANSPARENT;
    };

    using Colour = Color;

}
