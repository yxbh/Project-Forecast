#pragma once

#include "../Graphics/GraphicsCommand.hpp"

#include <memory>

namespace ke
{

    class IGraphicsCommandRenderer
    {
    public:
        using Sptr = std::shared_ptr<IGraphicsCommandRenderer>;
        using Uptr = std::unique_ptr<IGraphicsCommandRenderer>;
        using Wptr = std::weak_ptr<IGraphicsCommandRenderer>;

        virtual ~IGraphicsCommandRenderer(void) {}

        virtual void queueCommand(const GraphicsCommand & command) = 0;
        virtual void render() = 0;
        virtual void flush() = 0;
        virtual size_t getLastDrawCallCount() const = 0;
    };

    using GraphicsCommandRendererSptr = IGraphicsCommandRenderer::Sptr;
    using GraphicsCommandRendererUptr = IGraphicsCommandRenderer::Uptr;
    using GraphicsCommandRendererWptr = IGraphicsCommandRenderer::Wptr;

}