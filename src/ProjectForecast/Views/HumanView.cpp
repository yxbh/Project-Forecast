#include "HumanView.hpp"

#include "../InputControllers/InputControllers.hpp"

#include "KEngine/Graphics/Scene.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Log/Log.hpp"


namespace pf
{
    static auto logger = ke::Log::createDefaultLogger("pf::HumanView");

    HumanView::HumanView()
    {
        mouseController = std::make_unique<pf::MouseInputController>();
        keyboardController = std::make_unique<pf::KeyboardInputController>();

        scene = std::make_unique<ke::Scene>();

        ke::EventManager::registerListener<ke::MouseButtonPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::MouseButtonReleasedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::MouseWheelScrolledEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::MouseMovedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::KeyboardKeyPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::KeyboardKeyReleasedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::KeyboardTextEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::registerListener<ke::WindowResizedEvent>(this, &HumanView::handleWindowEvent);
    }

    HumanView::~HumanView()
    {
        ke::EventManager::deregisterListener<ke::MouseButtonPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::MouseButtonReleasedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::MouseWheelScrolledEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::MouseMovedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::KeyboardKeyPressedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::KeyboardKeyReleasedEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::KeyboardTextEvent>(this, &HumanView::handleWindowEvent);
        ke::EventManager::deregisterListener<ke::WindowResizedEvent>(this, &HumanView::handleWindowEvent);
    }

    void HumanView::attachEntity(ke::EntityId entityId)
    {
        this->ke::HumanView::attachEntity(entityId);
        static_cast<pf::KeyboardInputController*>(this->keyboardController.get())->attachEntity(entityId);
    }

    void HumanView::update(ke::Time p_elapsedTime)
    {
        assert(mouseController);
        assert(keyboardController);
        this->mouseController->update(p_elapsedTime);
        this->keyboardController->update(p_elapsedTime);
        this->getScene()->update(p_elapsedTime);
    }

    void HumanView::handleWindowEvent(ke::EventSptr event)
    {
        switch (event->getType())
        {
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

            case ke::MouseWheelScrolledEvent::TYPE:
            {
                auto mouseWheelEvent = std::static_pointer_cast<ke::MouseWheelScrolledEvent>(event);
                mouseController->onWheelScrolled(mouseWheelEvent->getDetail());
                break;
            }

            case ke::MouseMovedEvent::TYPE:
            {
                auto mouseEvent = std::static_pointer_cast<ke::MouseMovedEvent>(event);
                mouseController->onPointerMoved(mouseEvent->getDetail());
                break;
            }

            case ke::KeyboardKeyPressedEvent::TYPE:
            {
                auto keyboardEvent = static_cast<ke::KeyboardKeyPressedEvent*>(event.get());
                keyboardController->onKeyPressed(keyboardEvent->getDetail());
                //if (keyboardEvent->getDetail().keyCode == ke::Keyboard::Return)
                //{
                //    logger->info("echo({}): {}", testTextBuffer.length(), testTextBuffer);
                //    testTextBuffer.clear();
                //}
                //else
                //{
                //    keyboardController->onKeyReleased(keyboardEvent->getDetail());
                //}
                break;
            }

            case ke::KeyboardKeyReleasedEvent::TYPE:
            {
                auto keyboardEvent = std::static_pointer_cast<ke::KeyboardKeyReleasedEvent>(event);
                keyboardController->onKeyReleased(keyboardEvent->getDetail());
                break;
            }

            case ke::KeyboardTextEvent::TYPE:
            {
                //auto textEvent = static_cast<ke::KeyboardTextEvent*>(event.get());
                //// we only care about printing ASCII characters between 32 and 127.
                //if (textEvent->getDetail().unicode >= 32 &&
                //    textEvent->getDetail().unicode < 127)
                //{
                //    testTextBuffer = testTextBuffer + static_cast<char>(textEvent->getDetail().unicode);
                //}
                break;
            }

            case ke::WindowResizedEvent::TYPE:
            {
                logger->info("Window resized.");
                break;
            }
        }
        
    }
}
