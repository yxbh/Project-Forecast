#pragma once

#include "KEngine/Interface/IEvent.hpp"

namespace ke
{

    /// <summary>
    /// An error event that indicates the graphics loop failed to start correctly.
    /// </summary>
    class GraphicsLoopSetupFailureEvent : public ke::IEvent
    {
    public:
        static const EventType TYPE = 0xA5E40CB2;

        using ke::IEvent::IEvent;

        virtual ke::EventType getType() const final
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("GraphicsLoopSetupFailureEvent");
        }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<GraphicsLoopSetupFailureEvent>();
        }

    private:
    };
    
}
