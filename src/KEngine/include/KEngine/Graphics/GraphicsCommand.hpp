#pragma once
#pragma once

#include "KEngine/Common/Color.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Transform2D.hpp"


namespace ke::graphics
{

    struct TextureLoadCommandProperty
    {
        void * data = nullptr; // pointer to CPU image data.
        std::uint16_t textureId = 0;
    };

    struct RenderCommandProperty
    {
        Point2D origin;
        Transform2D globalTransform;
        Transform2D deltaTransform;

        std::uint8_t depth = 0;
        std::uint16_t textureId = 0;

        ke::Color fillColor;
        ke::Color outlineColor;
        float outlineThickness = 0.0;
        float radius;
    };

}

namespace ke
{

    struct GraphicsCommand
    {

        enum Types : std::uint8_t
        {
            Invalid = 0,
            GenerateTexture,
            RenderInvisible,
            RenderCircleShape,
            RenderSquareShape,
            RenderConvexShape,
            RenderSprite
        };

        Types type = Types::Invalid;

        union
        {
            ke::graphics::TextureLoadCommandProperty texture;
            ke::graphics::RenderCommandProperty render;
        };

        GraphicsCommand() {}
    };

}