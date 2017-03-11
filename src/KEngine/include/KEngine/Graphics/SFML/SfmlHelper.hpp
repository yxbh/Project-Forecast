#pragma once

#include "KEngine/Common/Color.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ke
{

    /// <summary>
    /// A helper static class that provides helper functions for doing 1 to 1
    /// conversions between matching KEngine and SFML data types.
    /// </summary>
    class SfmlHelper
    {
    public:
        static sf::Color convert(const ke::Color & keColor)
        {
            return {keColor.r, keColor.g, keColor.b, keColor.a};
        }

        static sf::Vector2f convert(const ke::Point2DFloat & point)
        {
            return { point.x, point.y };
        }

    };

}