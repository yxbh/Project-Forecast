#pragma once

#include "KEngine/Interfaces/IEvent.hpp"

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
        /// <param name="sfEvent">SFML event object.</param>
        /// <returns>nullptr if there is no matching KEngine.</returns>
        static ke::EventSptr translate(const sf::Event & sfEvent, sf::RenderWindow * const renderWindow);

        SfmlEventTranslator() = delete;
    };

}