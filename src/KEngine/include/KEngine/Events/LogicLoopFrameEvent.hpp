#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// 
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
            return std::make_shared<LogicLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
