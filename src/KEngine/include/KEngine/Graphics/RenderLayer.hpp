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
        using GraphicsCommandList = std::vector<GraphicsCommand>;

        ke::graphics::DepthType depth;
        GraphicsCommandList graphicsCommands;
    };

}