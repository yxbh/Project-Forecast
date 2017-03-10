#pragma once

#include "KEngine/Common/Color.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Transform2D.hpp"

#include <cstdint>

namespace ke::graphics
{

    struct ViewContextCommandProperty
    {
        ke::Transform2D transform;
        ke::Point2DInt32 dimension;
    };

    struct TextureLoadCommandProperty
    {
        void * data = nullptr; // pointer to CPU image data.
        std::uint16_t textureId = 0;
    };

    using IdType = std::uint32_t;

    struct RenderCommandProperty
    {
        IdType id = 0;

        Point2DFloat origin;
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
            SetViewContext,
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
            ke::graphics::RenderCommandProperty render;
            ke::graphics::ViewContextCommandProperty view;
            ke::graphics::TextureLoadCommandProperty texture;
        };

        GraphicsCommand() {}
    };

}
