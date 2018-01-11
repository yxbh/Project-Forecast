#include "InputControllers.hpp"

#include "../Events/RequestDrawDebugDotEvent.hpp"
#include "../Events/GMSRoomLoadRequestEvent.hpp"

#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/EntityTransformControlCommandEvents.hpp"
#include "KEngine/Events/AppEvents.hpp"


namespace pf
{

    void MouseInputController::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    bool MouseInputController::onButtonPressed(const ke::Mouse::ButtonInfo & buttonInfo)
    {
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
        return false;
    }

    bool MouseInputController::onPointerMoved()
    {
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
        return false;
    }

    bool KeyboardInputController::onKeyReleased(const ke::Keyboard::KeyInfo & keyInfo)
    {
        if (keyInfo.keyCode == ke::Keyboard::Add)
        {
            ke::EventManager::enqueue(ke::makeEvent<pf::GMSRoomLoadRequestEvent>("r1_1_1"));
            return true;
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