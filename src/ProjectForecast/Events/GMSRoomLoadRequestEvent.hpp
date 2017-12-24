#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Common/String.hpp"

namespace pf
{

    /// <summary>
    /// Event for requesting to load a GM:S room as a map/scene.
    /// </summary>
    class GMSRoomLoadRequestEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(GMSRoomLoadRequestEvent, 0x75f7accd)

    public:
        GMSRoomLoadRequestEvent(const ke::String & roomName)
            : roomName(roomName)
        {
        }

        virtual ke::EventSptr makeCopy() const override
        {
            return ke::makeEvent<GMSRoomLoadRequestEvent>(this->roomName);
        }

        inline const ke::String & getRoomName() const { return this->roomName; }

    private:
        ke::String roomName;

    };

}
