#pragma once

#include "KEngine/Common/Color.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Rect2D.hpp"
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
        size_t textureId = 0;
    };

    using IdType = size_t;
    using DepthType = int32_t;

    /// <summary>
    /// A header struct that maps the common properties of all *RenderCommandProperty classes.
    /// </summary>
    struct RenderCommandPropertyHeader
    {
        IdType id = 0;

        DepthType depth = 0;
    };

    struct ShapeRenderCommandProperty : public RenderCommandPropertyHeader
    {
        Point2DFloat origin;
        Transform2D globalTransform;

        ke::Color fillColor;
        ke::Color outlineColor;
        float outlineThickness = 0.0;
        float radius;
    };

    struct LineRenderCommandProperty : public RenderCommandPropertyHeader
    {
        Point2DFloat begin;
        Point2DFloat end;

        ke::Color color;
    };

    struct SpriteRenderCommandProperty : public RenderCommandPropertyHeader
    {
        Point2DFloat origin;
        Transform2D globalTransform;
        Rect2DInt32 textureRect;

        size_t textureId = 0;
        ke::Color color;
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
            RenderLine,
            RenderCircleShape,
            RenderSquareShape,
            RenderConvexShape,
            RenderSprite
        };

        Types type = Types::Invalid;

        union
        {
            ke::graphics::LineRenderCommandProperty   render;
            ke::graphics::LineRenderCommandProperty   line;
            ke::graphics::ShapeRenderCommandProperty  shape;
            ke::graphics::SpriteRenderCommandProperty sprite;
            ke::graphics::ViewContextCommandProperty  view;
            ke::graphics::TextureLoadCommandProperty  texture;
        };

        GraphicsCommand() {}
    };

}
