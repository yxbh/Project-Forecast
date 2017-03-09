#include "HumanView.hpp"

#include "../InputControllers/InputControllers.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Events/SFML/SfmlEvent.hpp"
#include "KEngine/Log/Log.hpp"

#include <SFML/Window/Event.hpp>

namespace pf
{

    HumanView::HumanView()
    {
        mouseController = std::make_unique<pf::MouseInputController>();
        keyboardController = std::make_unique<pf::KeyboardInputController>();

        scene = std::make_unique<ke::Scene>();

        ke::EventManager::registerListener<ke::SfmlEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::MouseButtonPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::MouseButtonReleasedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::KeyboardKeyPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::KeyboardKeyReleasedEvent>(this, &HumanView::handleWindowEvent);
    }

    HumanView::~HumanView()
    {
        ke::EventManager::deregisterListener<ke::SfmlEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::MouseButtonPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::MouseButtonReleasedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::KeyboardKeyPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::KeyboardKeyReleasedEvent>(this, &HumanView::handleWindowEvent);
    }

    void HumanView::attachEntity(ke::EntityId entityId)
    {
        this->ke::HumanView::attachEntity(entityId);
        static_cast<pf::KeyboardInputController*>(this->keyboardController.get())->attachEntity(entityId);
    }

    void HumanView::update(ke::Time elapsedTime)
    {
        assert(mouseController);
        assert(keyboardController);
        mouseController->update(elapsedTime);
        keyboardController->update(elapsedTime);
    }

    void HumanView::handleWindowEvent(ke::EventSptr event)
    {
        switch (event->getType())
        {
            case ke::SfmlEvent::TYPE:
            {
                auto windowEvent = std::static_pointer_cast<ke::SfmlEvent>(event);
                this->handleSfmlEvent(windowEvent->getSfmlEvent());
                break;
            }

            case ke::MouseButtonPressedEvent::TYPE:
            {
                auto mouseEvent = std::static_pointer_cast<ke::MouseButtonPressedEvent>(event);
                mouseController->onButtonPressed(mouseEvent->getDetail());
                break;
            }

            case ke::MouseButtonReleasedEvent::TYPE:
            {
                auto mouseEvent = std::static_pointer_cast<ke::MouseButtonReleasedEvent>(event);
                mouseController->onButtonReleased(mouseEvent->getDetail());
                break;
            }

            case ke::KeyboardKeyPressedEvent::TYPE:
            {
                auto keyboardEvent = std::static_pointer_cast<ke::KeyboardKeyPressedEvent>(event);
                keyboardController->onKeyPressed(keyboardEvent->getDetail());
                break;
            }

            case ke::KeyboardKeyReleasedEvent::TYPE:
            {
                auto keyboardEvent = std::static_pointer_cast<ke::KeyboardKeyReleasedEvent>(event);
                keyboardController->onKeyReleased(keyboardEvent->getDetail());
                break;
            }
        }
        
    }

    void HumanView::handleSfmlEvent(const sf::Event & event)
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                ke::Log::instance()->info("echo({}): {}", testTextBuffer.length(), testTextBuffer);
                testTextBuffer.clear();
            }
            break;
        }

        case sf::Event::TextEntered:
        {
            if (event.text.unicode >= 32 && event.text.unicode < 127) // we only care about printing ASCII characters between 32 and 127.
            {
                testTextBuffer = testTextBuffer + static_cast<char>(event.text.unicode);
            }
            break;
        }        

        default:
            break;
        }
    }
}
