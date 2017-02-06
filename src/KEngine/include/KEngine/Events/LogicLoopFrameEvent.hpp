#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// A heart beat event that is sent once every frame in the logic loop.
    /// </summary>
    class LogicLoopFrameEvent : public ke::LoopFrameEvent
    {
    public:
        static const EventType TYPE = 0x4A6EE2CF;

        using ke::LoopFrameEvent::LoopFrameEvent;

        virtual ke::EventType getType() const final
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("LogicLoopFrameEvent");
        }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<LogicLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
