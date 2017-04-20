#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Common/Time.hpp"

namespace ke
{
    
    /// <summary>
    /// 
    /// </summary>
    class LoopFrameEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(LoopFrameEvent, 0xC61C500)

    public:
        LoopFrameEvent(const ke::Time & timespan) : frameTimeSpan(timespan) {}

        inline const ke::Time & getFrameTimeSpan() const
        {
            return this->frameTimeSpan;
        }

    private:
        ke::Time frameTimeSpan;
    };

}