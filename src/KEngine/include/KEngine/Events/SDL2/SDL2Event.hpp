#pragma once

#include "KEngine/Interface/IEvent.hpp"

#include <SDL_events.h>

namespace ke
{

    /// <summary>
    /// Event class that represents and contains a SDL2 SDL_Event object.
    /// </summary>
    class SDL2Event : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(SDL2Event)
    public:
        static const EventType TYPE = 0xE281D216;

        SDL2Event(const SDL_Event & event) : sdlEvent(event)
        {

        }

        virtual ke::EventSptr makeCopy() const final
        {
            return std::make_shared<SDL2Event>(this->sdlEvent);
        }

        inline const SDL_Event & getSdl2Event() const
        {
            return this->sdlEvent;
        }

    private:
        SDL_Event sdlEvent;
    };

}
