#include "InputControllers.hpp"

#include "../Events/AppConsoleEvents.hpp"
#include "../Events/RequestDrawDebugDotEvent.hpp"
#include "../Events/GMSRoomLoadRequestEvent.hpp"
#include "../Events/OtherGraphicsEvents.hpp"

#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/EntityTransformControlCommandEvents.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Log/Log.hpp"


namespace pf
{

    void MouseInputController::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
        if (this->isAppWantCaptureMouse()) return;
    }

    bool MouseInputController::onButtonPressed(const ke::Mouse::ButtonInfo & buttonInfo)
    {
        if (this->isAppWantCaptureMouse()) return false;
        switch (buttonInfo.button)
        {
        case ke::Mouse::Button::Left:
            ke::EventManager::enqueue(ke::makeEvent<pf::RequestDrawDebugDotEvent>(buttonInfo.worldPosition));
            return true;
        }
        return false;
    }

    bool MouseInputController::onButtonReleased(const ke::Mouse::ButtonInfo & buttonInfo)
    {
        KE_UNUSED(buttonInfo);
        if (this->isAppWantCaptureMouse()) return false;
        return false;
    }

    bool MouseInputController::onWheelScrolled(const ke::Mouse::ScrollWheelInfo & scrollWheelInfo)
    {
        if (this->isAppWantCaptureMouse()) return false;
        if (scrollWheelInfo.scrollWhell == ke::Mouse::ScrollWheel::Vertical)
        {
            ke::EventManager::enqueue(ke::makeEvent<ke::CameraViewZoomDeltaRequestEvent>(scrollWheelInfo.delta));
            return true;
        }
        return false;
    }

    bool MouseInputController::onPointerMoved(const ke::Mouse::MouseInfo & mouseInfo)
    {
        KE_UNUSED(mouseInfo);
        if (this->isAppWantCaptureMouse()) return false;
        return false;
    }


    KeyboardInputController::KeyboardInputController(ke::EntityId p_entityId)
        : entityId(p_entityId)
    {
        ke::EventManager::registerListener<ke::WindowFocusGainedEvent>(this, &KeyboardInputController::processEvent);
        ke::EventManager::registerListener<ke::WindowFocusLostEvent>(this, &KeyboardInputController::processEvent);
    }

    KeyboardInputController::~KeyboardInputController()
    {
        ke::EventManager::deregisterListener<ke::WindowFocusGainedEvent>(this, &KeyboardInputController::processEvent);
        ke::EventManager::deregisterListener<ke::WindowFocusLostEvent>(this, &KeyboardInputController::processEvent);
    }

    void KeyboardInputController::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
        if (this->isAppWantCaptureKeyboard()) return;

        ke::Transform2D deltaTransform;
        bool moveRequested = false;

        if (this->isWindowInFocus)
        {
            if (ke::Keyboard::isKeyPressed(ke::Keyboard::Key::Left))
            {
                deltaTransform.x -= 5;
                moveRequested = true;
            }
            if (ke::Keyboard::isKeyPressed(ke::Keyboard::Key::Right))
            {
                deltaTransform.x += 5;
                moveRequested = true;
            }
            if (ke::Keyboard::isKeyPressed(ke::Keyboard::Key::Up))
            {
                deltaTransform.y += 5;
                moveRequested = true;
            }
            if (ke::Keyboard::isKeyPressed(ke::Keyboard::Key::Down))
            {
                deltaTransform.y -= 5;
                moveRequested = true;
            }
        }

        if (moveRequested)
        {
            ke::EventManager::enqueue(
                ke::makeEvent<ke::EntityTransformDeltaControlCommandEvent>(this->entityId, deltaTransform));
        }        
    }

    bool KeyboardInputController::onKeyPressed(const ke::Keyboard::KeyInfo & keyInfo)
    {
        KE_UNUSED(keyInfo);
        if (this->isAppWantCaptureKeyboard()) return false;
        return false;
    }

    bool KeyboardInputController::onKeyReleased(const ke::Keyboard::KeyInfo & keyInfo)
    {
        switch (keyInfo.keyCode)
        {
        case ke::Keyboard::Tilde:
        {
            if (!this->isAppWantCaptureTextInput())
            {
                ke::EventManager::enqueue(ke::makeEvent<ke::RequestToggleAppConsoleDisplayEvent>());
                return true;
            }
            break;
        }
        case ke::Keyboard::Add:
        {
            ke::EventManager::enqueue(ke::makeEvent<pf::GMSRoomLoadRequestEvent>("r1_1_1"));
            return true;
        }
        case ke::Keyboard::Subtract:
        {
            ke::EventManager::enqueue(ke::makeEvent<pf::GMSRoomLoadRequestEvent>("")); // empty room name for unloading room only.
            return true;
        }
        }
        return false;
    }

    void KeyboardInputController::processEvent(ke::EventSptr event)
    {
        switch (event->getType())
        {
        case ke::WindowFocusGainedEvent::TYPE:
        {
            this->isWindowInFocus = true;
            break;
        }
        case ke::WindowFocusLostEvent::TYPE:
        {
            this->isWindowInFocus = false;
            break;
        }
        default: { /* do nothing. */ }
        }
    }

}