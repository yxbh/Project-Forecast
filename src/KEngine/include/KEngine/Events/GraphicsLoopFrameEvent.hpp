#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// A heart beat event that is sent once every frame in the render loop.
    /// </summary>
    class GraphicsLoopFrameEvent : public ke::LoopFrameEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(GraphicsLoopFrameEvent, 0xE540161D)
    
    public:
        using ke::LoopFrameEvent::LoopFrameEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<GraphicsLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
