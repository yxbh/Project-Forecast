#pragma once

#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Common/Time.hpp"

namespace ke
{
    
    /// <summary>
    /// 
    /// </summary>
    class LoopFrameEvent : public ke::IEvent
    {
    public:
        static const EventType TYPE = 0xC61C500;

        LoopFrameEvent(const ke::Time & timespan) : frameTimeSpan(timespan) {}

        virtual ke::EventType getType() const override
        {
            return TYPE;
        }

        inline const ke::Time & getFrameTimeSpan() const
        {
            return this->frameTimeSpan;
        }

    private:
        ke::Time frameTimeSpan;
    };

}