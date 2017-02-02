#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class EventLoopFrameEvent : public ke::LoopFrameEvent
    {
    public:
        static const EventType TYPE = 0xB058ACFE;

        using ke::LoopFrameEvent::LoopFrameEvent;

        virtual ke::EventType getType() const final
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("EventLoopFrameEvent");
        }

        virtual ke::EventSptr makeCopy() const final
        {
            return std::make_shared<EventLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
