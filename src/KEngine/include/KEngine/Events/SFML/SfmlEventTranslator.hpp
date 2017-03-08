#pragma once

#include "KEngine/Interface/IEvent.hpp"

#include <SFML/Window/Event.hpp>

namespace sf
{
    class RenderWindow;
}

namespace ke
{
    
    class SfmlEventTranslator
    {
    public:
        /// <summary>
        /// Translate the given sf::Event to a matching KEngine event if there is one.
        /// Otherwise nullptr is returned.
        /// </summary>
        /// <param name="event"></param>
        /// <returns>nullptr if there is no matching KEngine.</returns>
        static ke::EventSptr translate(const sf::Event & event, sf::RenderWindow * const renderWindow);

        SfmlEventTranslator() = delete;
    };

}