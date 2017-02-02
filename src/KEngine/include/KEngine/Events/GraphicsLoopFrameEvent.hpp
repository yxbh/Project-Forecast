#pragma once

#include "KEngine/Events/LoopFrameEvent.hpp"

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class GraphicsLoopFrameEvent : public ke::LoopFrameEvent
    {
    public:
        static const EventType TYPE = 0xE540161D;

        using ke::LoopFrameEvent::LoopFrameEvent;

        virtual ke::EventType getType() const final
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("GraphicsLoopFrameEvent");
        }

        virtual ke::EventSptr makeCopy() const final
        {
            return std::make_shared<GraphicsLoopFrameEvent>(this->getFrameTimeSpan());
        }
    };

}
