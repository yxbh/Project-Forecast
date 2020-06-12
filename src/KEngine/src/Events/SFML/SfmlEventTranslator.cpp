#include "KEngine/Events/SFML/SfmlEventTranslator.hpp"

#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Log/Log.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace ke
{
    static auto logger = ke::Log::createDefaultLogger("KEngine");

    ke::EventSptr SfmlEventTranslator::translate(const sf::Event & sfEvent, sf::RenderWindow * const renderWindow)
    {
        switch (sfEvent.type)
        {
        case sf::Event::MouseButtonPressed:
        {
            assert(renderWindow);
            const auto sfWorldCoord = renderWindow->mapPixelToCoords
            (
                { sfEvent.mouseButton.x, sfEvent.mouseButton.y },
                renderWindow->getView()
            );
            ke::Mouse::ButtonInfo eventInfo
            {
                ke::Mouse::mapInternalApiButtonToKeButton(sfEvent.mouseButton.button),
                { sfEvent.mouseButton.x , sfEvent.mouseButton.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseButtonPressedEvent>(eventInfo);
        }

        case sf::Event::MouseButtonReleased:
        {
            assert(renderWindow);
            const auto sfWorldCoord = renderWindow->mapPixelToCoords
            (
                { sfEvent.mouseButton.x, sfEvent.mouseButton.y },
                renderWindow->getView()
            );
            ke::Mouse::ButtonInfo eventInfo
            {
                ke::Mouse::mapInternalApiButtonToKeButton(sfEvent.mouseButton.button),
                { sfEvent.mouseButton.x , sfEvent.mouseButton.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseButtonReleasedEvent>(eventInfo);
        }

        case sf::Event::EventType::MouseWheelScrolled:
        {
            assert(renderWindow);
            const auto sfWorldCoord = renderWindow->mapPixelToCoords
            (
                { sfEvent.mouseButton.x, sfEvent.mouseButton.y },
                renderWindow->getView()
            );
            ke::Mouse::ScrollWheelInfo eventInfo
            {
                sfEvent.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel ? ke::Mouse::ScrollWheel::Vertical : ke::Mouse::ScrollWheel::Horizontal,
                sfEvent.mouseWheelScroll.delta,
                { sfEvent.mouseWheelScroll.x , sfEvent.mouseWheelScroll.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseWheelScrolledEvent>(eventInfo);
        }

        case sf::Event::EventType::MouseMoved:
        {
            assert(renderWindow);
            const auto sfWorldCoord = renderWindow->mapPixelToCoords
            (
                { sfEvent.mouseButton.x, sfEvent.mouseButton.y },
                renderWindow->getView()
            );
            ke::Mouse::MouseInfo eventInfo
            {
                { sfEvent.mouseWheelScroll.x , sfEvent.mouseWheelScroll.y },
                { sfWorldCoord.x , -sfWorldCoord.y }
            };
            return ke::makeEvent<ke::MouseMovedEvent>(eventInfo);
        }

        case sf::Event::EventType::MouseEntered:
        {
            return ke::makeEvent<ke::MouseEnteredEvent>();
        }
        case sf::Event::EventType::MouseLeft:
        {
            return ke::makeEvent<ke::MouseLeftEvent>();
        }
        case sf::Event::EventType::MouseWheelMoved:
        {
            logger->warn("Untranslated SFML event: sf::Event::EventType::MouseWheelMoved");
            return nullptr;
        }

        case sf::Event::KeyPressed:
        {
            using CodeType = ke::Keyboard::KeyInfo::CodeType;
            ke::Keyboard::KeyInfo eventInfo
            {
                static_cast<CodeType>(sfEvent.key.code),
                ke::Keyboard::NULL_KEYBOARD_SCANCODE,
                sfEvent.key.control,
                sfEvent.key.alt,
                sfEvent.key.shift,
                sfEvent.key.system
            };
            return ke::makeEvent<ke::KeyboardKeyPressedEvent>(eventInfo);
        }

        case sf::Event::KeyReleased:
        {
            using CodeType = ke::Keyboard::KeyInfo::CodeType;
            ke::Keyboard::KeyInfo eventInfo
            {
                static_cast<CodeType>(sfEvent.key.code),
                ke::Keyboard::NULL_KEYBOARD_SCANCODE,
                sfEvent.key.control,
                sfEvent.key.alt,
                sfEvent.key.shift,
                sfEvent.key.system
            };
            return ke::makeEvent<ke::KeyboardKeyReleasedEvent>(eventInfo);
        }

        case sf::Event::TextEntered:
        {
            ke::Keyboard::TextInfo detail{ sfEvent.text.unicode };
            return ke::makeEvent<ke::KeyboardTextEvent>(detail);
        }

        case sf::Event::EventType::Resized:
        {
            ke::Dimension2DUInt32 newSize{ sfEvent.size.width, sfEvent.size.height };
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

        case sf::Event::EventType::Closed:
        {
            logger->warn("Untranslated SFML event: sf::Event::EventType::Closed");
            return nullptr;
        }

        default:
        {
            logger->warn("Untranslated SFML event: {}", sfEvent.type);
            return nullptr;
        }
        }  // switch sf::Event type.
    }

}