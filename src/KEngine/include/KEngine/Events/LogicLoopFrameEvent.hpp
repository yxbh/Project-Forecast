#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// A heart beat event that is sent once every frame in the logic loop.
    /// </summary>
    class LogicLoopFrameEvent : public ke::LoopFrameEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(LogicLoopFrameEvent, 0x4A6EE2CF)
    
    public:
        using ke::LoopFrameEvent::LoopFrameEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<LogicLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
