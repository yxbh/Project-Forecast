#include "InputControllers.hpp"

#include "../Events/RequestDrawDebugDotEvent.hpp"

#include "KEngine/Core/EventManager.hpp"


namespace pf
{


    void MouseInputController::update(ke::Time elapsedTime)
    {

    }

    bool MouseInputController::onButtonPressed(ke::MouseButton button, const ke::Point2D & position)
    {
        switch (button)
        {
        case ke::MouseButton::Left:
            ke::EventManager::enqueue(ke::makeEvent<pf::RequestDrawDebugDotEvent>(position));
            return true;
        }
        return false;
    }

    bool MouseInputController::onButtonReleased(ke::MouseButton button, const ke::Point2D & position)
    {
        return false;
    }

    bool MouseInputController::onPointerMoved()
    {
        return false;
    }


    void KeyboardInputController::update(ke::Time elapsedTime)
    {

    }

    bool KeyboardInputController::onKeyPressed()
    {
        return false;
    }

    bool KeyboardInputController::onKeyReleased()
    {
        return false;
    }

}