#pragma once

#include "../Graphics/GraphicsCommand.hpp"

namespace ke
{

    class IGraphicsCommandRenderer
    {
    public:
        virtual void queueCommand(const GraphicsCommand & command) = 0;
        virtual void render() = 0;
        virtual void flush() = 0;
        virtual size_t getLastDrawCallCount() const = 0;
    };

}