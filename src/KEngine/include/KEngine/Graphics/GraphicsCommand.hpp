#pragma once

#include "KEngine/Common/Color.hpp"
#include "KEngine/Common/Dimension2D.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Rect2D.hpp"
#include "KEngine/Common/Transform2D.hpp"

#include <cassert>
#include <cstdint>
#include <functional>
#include <variant>

namespace ke::graphics
{
    using DepthType = int32_t;

    struct ViewContextInfo
    {
        ke::Transform2D transform;
        ke::Dimension2DUInt32 dimension;
    };

    struct TextureLoadInfo
    {
        void * data = nullptr; // pointer to CPU image data.
        size_t textureId = 0;
    };

    struct CircleShapeRenderInfo
    {
        Point2DFloat origin;
        Transform2D globalTransform;

        ke::Color fillColor;
        ke::Color outlineColor;
        float outlineThickness = 0.0;
        float radius;
    };

    struct RectangleShapeRenderInfo
    {
        Point2DFloat origin;
        Transform2D globalTransform;

        // incomplete
    };

    struct ConvexShapeRenderInfo
    {
        Point2DFloat origin;
        Transform2D globalTransform;

        // incomplete
    };

    struct LineRenderInfo
    {
        Point2DFloat begin;
        Point2DFloat end;

        ke::Color color;
    };

    struct SpriteRenderInfo
    {
        Point2DFloat origin;
        Transform2D globalTransform;
        Rect2DInt32 textureRect;

        size_t textureId = 0;
        ke::Color color;
    };

    struct ImguiRenderInfo
    {
        using FunctorType = std::function<void(void)>;
        FunctorType imguiDrawer;
    };

}

namespace ke
{

    struct GraphicsCommand
    {
        enum class Types : std::uint8_t
        {
            // NOTE: each value below does not have to match the index of what's in the `info` variant below.
            Invalid             = 0,
            SetViewContext      = 1,
            GenerateTexture     = 2,
            DrawLine            = 3,
            DrawCircleShape     = 4,
            DrawRectangleShape  = 5,
            DrawConvexShape     = 6,
            DrawSprite          = 7,
            DrawImgui           = 8,
            DrawInvisible       = std::numeric_limits<std::uint8_t>::max() - 1,
            Count
        };

        Types type = Types::Invalid;

        using IdType    = size_t;
        using DepthType = ke::graphics::DepthType;
        IdType      id      = 0;
        DepthType   depth   = 0;

        using CommandInfo = std::variant
            <
            std::monostate,
            ke::graphics::ViewContextInfo,
            ke::graphics::TextureLoadInfo,
            ke::graphics::LineRenderInfo,
            ke::graphics::CircleShapeRenderInfo,
            ke::graphics::RectangleShapeRenderInfo,
            ke::graphics::ConvexShapeRenderInfo,
            ke::graphics::SpriteRenderInfo,
            ke::graphics::ImguiRenderInfo
            >;
        CommandInfo info;
    };

}
