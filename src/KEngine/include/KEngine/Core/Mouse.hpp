#pragma once

#include "KEngine/Common/Point2D.hpp"

#if defined(USE_SFML)
#include <SFML/Window/Mouse.hpp>
#endif

#include <cstdint>

namespace ke
{
    
    /// <summary>
    /// 
    /// </summary>
    class Mouse
    {
    public:
        /// <summary>
        /// Borrowed from SFML(2.4.2).
        /// TODO: research SDL implementation.
        /// </summary>
        enum Button : std::uint16_t
        {
            Left,       ///< The left mouse button
            Right,      ///< The right mouse button
            Middle,     ///< The middle (wheel) mouse button
            XButton1,   ///< The first extra mouse button
            XButton2,   ///< The second extra mouse button

            ButtonCount ///< Keep last -- the total number of mouse buttons
        };

        enum ScrollWheel : std::uint8_t
        {
            Vertical,
            Horizontal
        };

        struct MouseInfo
        {
            Point2DInt32 screenPosition;
            Point2DFloat worldPosition;
        };

        struct ButtonInfo
        {
            Button button;
            Point2DInt32 screenPosition;
            Point2DFloat worldPosition;
        };

        struct ScrollWheelInfo
        {
            ScrollWheel scrollWhell;
            float delta;
            Point2DInt32 screenPosition;
            Point2DFloat worldPosition;
        };


        static bool isButtonPressed(const ::ke::Mouse::Button button);

#if defined(USE_SFML)
        static ::ke::Mouse::Button mapInternalApiButtonToKeButton(const ::sf::Mouse::Button sfmlButton);
        static ::sf::Mouse::Button mapKeButtonToInternalSfmlButton(const ::ke::Mouse::Button keButton);
#endif
    };

}