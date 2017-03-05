#pragma once

#include "GraphicsCommand.hpp"

#include <cstdint>
#include <vector>

namespace ke
{

    /// <summary>
    /// A render layer represents a set of things to draw that belongs to a specific depth.
    /// </summary>
    class RenderLayer
    {
    public:
        using DepthType = std::int16_t;
        using GraphicsCommandList = std::vector<GraphicsCommand>;

        DepthType depth;
        GraphicsCommandList graphicsCommands;
    };

}