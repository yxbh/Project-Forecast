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
            const auto sfWorldCoord = renderWindow->mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y },
                renderWindow->getView());
            ke::Mouse::ButtonInfo eventInfo
            {
                ke::Mouse::mapInternalApiButtonToKeButton(event.mouseButton.button),
                { event.mouseButton.x , event.mouseButton.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseButtonPressedEvent>(eventInfo);
        }

        case sf::Event::MouseButtonReleased:
        {
            assert(renderWindow);
            const auto sfWorldCoord = renderWindow->mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y },
                renderWindow->getView());
            ke::Mouse::ButtonInfo eventInfo
            {
                ke::Mouse::mapInternalApiButtonToKeButton(event.mouseButton.button),
                { event.mouseButton.x , event.mouseButton.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseButtonReleasedEvent>(eventInfo);
        }

        case sf::Event::EventType::MouseWheelScrolled:
        {
            assert(renderWindow);
            const auto sfWorldCoord = renderWindow->mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y },
                renderWindow->getView());
            ke::Mouse::ScrollWheelInfo eventInfo
            {
                event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel ? ke::Mouse::ScrollWheel::Vertical : ke::Mouse::ScrollWheel::Horizontal,
                event.mouseWheelScroll.delta,
                { event.mouseWheelScroll.x , event.mouseWheelScroll.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseWheelScrolledEvent>(eventInfo);
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

        case sf::Event::TextEntered:
        {
            ke::Keyboard::TextInfo detail{ event.text.unicode };
            return ke::makeEvent<ke::KeyboardTextEvent>(detail);
        }

        case sf::Event::Resized:
        {
            ke::Dimension2DUInt32 newSize{ event.size.width, event.size.height };
            return ke::makeEvent<ke::WindowResizedEvent>(newSize);
        }

        case sf::Event::EventType::GainedFocus:
        {
            return ke::makeEvent<ke::WindowFocusGainedEvent>();
        }

        case sf::Event::EventType::LostFocus:
        {
            return ke::makeEvent<ke::WindowFocusLostEvent>();
        }

        default: return nullptr;
        } // switch sf::Event type.
    }

}