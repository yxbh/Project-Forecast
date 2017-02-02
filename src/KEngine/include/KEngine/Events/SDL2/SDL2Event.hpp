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
    public:
        static const EventType TYPE = 0xE281D216;

        SDL2Event(const SDL_Event & event) : sdlEvent(event)
        {

        }

        virtual ke::EventType getType() const final
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("SDL2Event");
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
