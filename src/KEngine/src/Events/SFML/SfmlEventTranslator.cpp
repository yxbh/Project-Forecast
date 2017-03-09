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
                { event.mouseButton.x, event.mouseButton.y },
                renderWindow->getView());
            ke::Mouse::ButtonInfo eventInfo{
                ke::Mouse::mapInternalApiButtonToKeButton(event.mouseButton.button),
                { event.mouseButton.x , event.mouseButton.y },
                { sfWorldCoord.x , -sfWorldCoord.y } };
            return ke::makeEvent<ke::MouseButtonPressedEvent>(eventInfo);
        }

        case sf::Event::MouseButtonReleased:
        {
            assert(renderWindow);
            auto sfWorldCoord = renderWindow->mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y },
                renderWindow->getView());
            ke::Mouse::ButtonInfo eventInfo{
                ke::Mouse::mapInternalApiButtonToKeButton(event.mouseButton.button),
                { event.mouseButton.x , event.mouseButton.y },
                { sfWorldCoord.x , -sfWorldCoord.y } };
            return ke::makeEvent<ke::MouseButtonReleasedEvent>(eventInfo);
        }

        case sf::Event::KeyPressed:
        {
            using CodeType = ke::Keyboard::KeyInfo::CodeType;
            ke::Keyboard::KeyInfo eventInfo{
                static_cast<CodeType>(event.key.code),
                ke::Keyboard::NULL_KEYBOARD_SCANCODE,
                event.key.control,
                event.key.alt,
                event.key.shift,
                event.key.system};
            return ke::makeEvent<ke::KeyboardKeyPressedEvent>(eventInfo);
        }

        case sf::Event::KeyReleased:
        {
            using CodeType = ke::Keyboard::KeyInfo::CodeType;
            ke::Keyboard::KeyInfo eventInfo{
                static_cast<CodeType>(event.key.code),
                ke::Keyboard::NULL_KEYBOARD_SCANCODE,
                event.key.control,
                event.key.alt,
                event.key.shift,
                event.key.system };
            return ke::makeEvent<ke::KeyboardKeyReleasedEvent>(eventInfo);
        }

        default: return nullptr;
        }
    }

}