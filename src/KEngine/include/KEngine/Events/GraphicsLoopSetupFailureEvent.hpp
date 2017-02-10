#pragma once

#include "KEngine/Interface/IEvent.hpp"

namespace ke
{

    /// <summary>
    /// An error event that indicates the graphics loop failed to start correctly.
    /// </summary>
    class GraphicsLoopSetupFailureEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(GraphicsLoopSetupFailureEvent)
    public:
        static const EventType TYPE = 0xA5E40CB2;

        using ke::IEvent::IEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<GraphicsLoopSetupFailureEvent>();
        }

    private:
    };
    
}
