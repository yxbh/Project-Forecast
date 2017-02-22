#pragma once
#pragma once

#include "KEngine/Common/Color.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Transform2D.hpp"

namespace ke
{

    enum RenderObjectType : std::uint8_t
    {
        Invisible = 0,
        Shape,
        CircleShape,
        SquareShape,
        ConvexShape,
        Sprite
    };

    struct RenderCommand
    {
        RenderObjectType type = Invisible;

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