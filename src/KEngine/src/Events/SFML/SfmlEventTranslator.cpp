#include "KEngine/Events/SFML/SfmlEventTranslator.hpp"

#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Log/Log.hpp"

#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace ke
{

    ke::EventSptr SfmlEventTranslator::translate(const sf::Event & event, sf::RenderWindow * const renderWindow)
    {
        if (!ImGui::GetIO().WantCaptureMouse)
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

            case sf::Event::EventType::MouseMoved:
            {
                assert(renderWindow);
                const auto sfWorldCoord = renderWindow->mapPixelToCoords(
                    { event.mouseButton.x, event.mouseButton.y },
                    renderWindow->getView());
                ke::Mouse::MouseInfo eventInfo
                {
                    { event.mouseWheelScroll.x , event.mouseWheelScroll.y },
                    { sfWorldCoord.x , -sfWorldCoord.y }
                };
                return ke::makeEvent<ke::MouseMovedEvent>(eventInfo);
            }

            case sf::Event::EventType::MouseEntered:
            {
                ke::Log::instance()->warn("Untranslated SFML event: sf::Event::EventType::MouseEntered");
                return nullptr;
            }
            case sf::Event::EventType::MouseLeft:
            {
                ke::Log::instance()->warn("Untranslated SFML event: sf::Event::EventType::MouseLeft");
                return nullptr;
            }
            case sf::Event::EventType::MouseWheelMoved:
            {
                ke::Log::instance()->warn("Untranslated SFML event: sf::Event::EventType::MouseWheelMoved");
                return nullptr;
            }
            }  // switch sf::Event type.
        }  // !ImGui::GetIO().WantCaptureMouse
        
        if (!ImGui::GetIO().WantCaptureKeyboard)
        {
            switch (event.type)
            {
            case sf::Event::KeyPressed:
            {
                using CodeType = ke::Keyboard::KeyInfo::CodeType;
                ke::Keyboard::KeyInfo eventInfo{
                    static_cast<CodeType>(event.key.code),
                    ke::Keyboard::NULL_KEYBOARD_SCANCODE,
                    event.key.control,
                    event.key.alt,
                    event.key.shift,
                    event.key.system };
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
            } // switch sf::Event type.
        }  // !ImGui::GetIO().WantCaptureKeyboard

        if (!ImGui::GetIO().WantTextInput)
        {
            switch (event.type)
            {
            case sf::Event::TextEntered:
            {
                ke::Keyboard::TextInfo detail{ event.text.unicode };
                return ke::makeEvent<ke::KeyboardTextEvent>(detail);
            }
            } // switch sf::Event type.
        }

        switch (event.type)
        {
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

        case sf::Event::EventType::Closed:
        {
            ke::Log::instance()->warn("Untranslated SFML event: sf::Event::EventType::Closed");
            return nullptr;
        }

        default: return nullptr;
        } // switch sf::Event type.
    }

}