#include "KEngine/Core/StateMachine.hpp"

namespace ke
{

    bool StateMachine::initialise()
    {
        // nothing to do.
        return true;
    }

    void StateMachine::shutdown()
    {
        // nothing to do.
    }

    void StateMachine::update(const ke::Time & elapsedTime)
    {
        assert(startState);
        assert(endState);

        if (!currentState)
        {
            currentState = startState;
        }

        currentState->update(elapsedTime);
    }

    void StateMachine::finishState(IState * state, int stateExitCode)
    {
        assert(this->currentState == state);
    }

    void StateMachine::addState(StateSptr state)
    {
        this->states.insert(state);
    }

    void StateMachine::addStartState(StateSptr state)
    {
        this->states.insert(state);
        startState = state.get();
    }

    void StateMachine::addEndState(StateSptr state)
    {
        this->states.insert(state);
        endState = state.get();
    }

    bool StateMachine::addStateTransition(StateMachineStateType exitStateType, StateMachineStateExitCode exitStateExitCode, StateMachineStateType startStateType)
    {
        // TODO : implementation.
        //stateTransitionMap[exitStateExitCode][exitStateExitCode]
        return false;
    }

}