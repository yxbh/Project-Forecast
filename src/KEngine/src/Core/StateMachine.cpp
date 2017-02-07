#include "KEngine/Core/StateMachine.hpp"

namespace ke
{

    bool StateMachine::initialise()
    {
        return true;
    }

    void StateMachine::shutdown()
    {

    }

    void StateMachine::begin()
    {
        for (auto & state : this->states)
        {
            state->initialise();
        }
    }

    void StateMachine::update(const ke::Time & elapsedTime)
    {
        if (!currentState)
        {
            return;
        }

        currentState->update(elapsedTime);
    }

}