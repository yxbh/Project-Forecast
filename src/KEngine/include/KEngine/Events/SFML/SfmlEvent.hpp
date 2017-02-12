#pragma once

#include "KEngine/Interface/IEvent.hpp"

#include <SFML/Window/Event.hpp>

namespace ke
{
    
    /// <summary>
    /// Event class that represents and contains a SFML sf::Event object.
    /// </summary>
    class SfmlEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(SfmlEvent)
    public:
        static const EventType TYPE = 0x17D63FB6;

        SfmlEvent(const sf::Event & event) : sfEvent(event)
        {

        }

        virtual ke::EventSptr makeCopy() const final
        {
            return std::make_shared<SfmlEvent>(this->sfEvent);
        }

        inline const sf::Event & getSfmlEvent() const
        {
            return this->sfEvent;
        }

    private:
        sf::Event sfEvent;
    };

}