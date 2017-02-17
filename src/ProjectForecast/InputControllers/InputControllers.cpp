#include "InputControllers.hpp"

#include "../Events/RequestDrawDebugDotEvent.hpp"

#include "KEngine/Core/EventManager.hpp"


namespace pf
{


    void MouseInputController::update(ke::Time elapsedTime)
    {

    }

    bool MouseInputController::onButtonPressed(ke::MouseButton button)
    {
        switch (button)
        {
        case ke::MouseButton::Left:
            ke::EventManager::enqueue(ke::makeEvent<pf::RequestDrawDebugDotEvent>());
            return true;
        }
        return false;
    }

    bool MouseInputController::onButtonReleased(ke::MouseButton button)
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