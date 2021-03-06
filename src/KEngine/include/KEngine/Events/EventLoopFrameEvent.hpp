#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// A heart beat event that is sent once every frame in the event loop.
    /// </summary>
    class EventLoopFrameEvent : public ke::LoopFrameEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(EventLoopFrameEvent, 0xB058ACFE)
    
    public:
        using ke::LoopFrameEvent::LoopFrameEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<EventLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
