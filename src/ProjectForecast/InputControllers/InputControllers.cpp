#include "InputControllers.hpp"

#include "../Events/RequestDrawDebugDotEvent.hpp"

#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/EntityTransformControlCommandEvents.hpp"


namespace pf
{

    void MouseInputController::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    bool MouseInputController::onButtonPressed(ke::Mouse::Button button, const ke::Point2DInt32 & position)
    {
        switch (button)
        {
        case ke::Mouse::Button::Left:
            ke::EventManager::enqueue(ke::makeEvent<pf::RequestDrawDebugDotEvent>(position));
            return true;
        }
        return false;
    }

    bool MouseInputController::onButtonReleased(ke::Mouse::Button button, const ke::Point2DInt32 & position)
    {
        KE_UNUSED(button);
        KE_UNUSED(position);
        return false;
    }

    bool MouseInputController::onPointerMoved()
    {
        return false;
    }


    void KeyboardInputController::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);

        ke::Transform2D deltaTransform;
        bool moveRequested = false;

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
        KE_UNUSED(keyInfo);
        return false;
    }

}