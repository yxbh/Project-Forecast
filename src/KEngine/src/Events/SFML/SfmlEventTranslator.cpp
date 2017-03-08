#include "KEngine/Events/SFML/SfmlEventTranslator.hpp"

#include "KEngine/Events/AppEvents.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace ke
{

    ke::EventSptr SfmlEventTranslator::translate(const sf::Event & event, sf::RenderWindow * const renderWindow)
    {
        
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
        {
            assert(renderWindow);
            auto sfWorldCoord = renderWindow->mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.x },
                renderWindow->getView());
            MouseEventInfo eventInfo{ { event.mouseButton.x , event.mouseButton.y }, { sfWorldCoord.x , sfWorldCoord.y } };
            auto keEvent = ke::makeEvent<ke::MouseButtonPressedEvent>(eventInfo);
            return keEvent;
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            assert(renderWindow);
            auto sfWorldCoord = renderWindow->mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.x },
                renderWindow->getView());
            MouseEventInfo eventInfo{ { event.mouseButton.x , event.mouseButton.y },{ sfWorldCoord.x , sfWorldCoord.y } };
            auto keEvent = ke::makeEvent<ke::MouseButtonReleasedEvent>(eventInfo);
            return keEvent;
            break;
        }

        default: return nullptr;
        }
    }

}