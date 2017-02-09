#include "KEngine/Core/StateMachine.hpp"

#include "KEngine/Log/Log.hpp"

#include <algorithm>

namespace ke
{

    void StateMachine::update(const ke::Time & elapsedTime)
    {
        assert(this->startState);
        assert(this->endState);

        // the state machine has not started or has been reset.
        if (!currentState)
        {
            this->currentState = this->startState;
			this->currentState->onEnter();
        }

        this->status = Status::Running;
        this->currentState->update(elapsedTime);


        // current state requested exit.
        if (this->isCurrentStateExitRequested)
        {
            this->isCurrentStateExitRequested = false;
            auto stateExitCode = this->currentStateExitCode;
            this->currentStateExitCode = -1;

            this->currentState->onExit();

            // we reset the state machine if the state requesting exit is the end state.
            if (this->currentState == this->endState)
            {
                this->status = Status::Finished;
                this->currentState = nullptr;
                return;
            }

            auto exitingStateType = this->currentState->getType();

            // no state transition from the current state.
            // implicitly transition directly to the end state.
            auto stateTransitionSrcStateType_it = this->stateTransitionMap.find(exitingStateType);
            if (stateTransitionSrcStateType_it == this->stateTransitionMap.end())
            {
#if defined(KE_DEBUG)
                ke::Log::instance()->warn("No transition from state of type {}. " \
                    "Implicitly transitioning directly to end state of type {}.",
                    this->currentState->getName(), this->endState->getName());
#endif
                this->currentState = this->endState;
                this->currentState->onEnter();
                return;
            }

            // state transition(s) available for current state but no mapping for given exit code.
            // implicitly transition directly to the end state.
            auto & exitCodeToDestStateMap = (*stateTransitionSrcStateType_it).second;
            auto stateTransitionSrcStateExitCode_it = exitCodeToDestStateMap.find(stateExitCode);
            if (stateTransitionSrcStateExitCode_it == exitCodeToDestStateMap.end())
            {
#if defined(KE_DEBUG)
                ke::Log::instance()->warn("No transition from state of type {} for exit code {}. " \
                    "Implicitly transitioning directly to end state of type {}.",
                    this->currentState->getName(), stateExitCode, this->endState->getName());
#endif
                this->currentState = this->endState;
                this->currentState->onEnter();
                return;
            }

            // mapping is available. transition to target state.
            auto destState = (*stateTransitionSrcStateExitCode_it).second;
            this->currentState = destState;
            this->currentState->onEnter();
        }
    }

    void StateMachine::finishState(IState * state, int stateExitCode)
    {
        assert(this->currentState == state);
		assert(this->startState);
		assert(this->endState);

        this->isCurrentStateExitRequested = true;
        this->currentStateExitCode = stateExitCode;
    }

    void StateMachine::addState(StateSptr state)
    {
		assert(state);
		assert(state->getType() != ke::INVALID_STATE_MACHINE_STATE_TYPE);
#if defined(KE_DEBUG)
		for (auto existingState : this->states)
		{
			if (existingState->getType() == state->getType())
			{
				ke::Log::instance()->error("A start state instance of {} already exists in this state machine.", state->getName());
			}
		}
#endif
        this->states.insert(state);
    }

    void StateMachine::addStartState(StateSptr state)
    {
		assert(state);
		assert(state->getType() != ke::INVALID_STATE_MACHINE_STATE_TYPE);
#if defined(KE_DEBUG)
		for (auto existingState : this->states)
		{
			if (existingState->getType() == state->getType())
			{
				ke::Log::instance()->error("A state instance of {} already exists in this state machine.", state->getName());
			}
		}
#endif
        this->states.insert(state);
		this->startState = state.get();
    }

    void StateMachine::addEndState(StateSptr state)
    {
		assert(state);
		assert(state->getType() != ke::INVALID_STATE_MACHINE_STATE_TYPE);
#if defined(KE_DEBUG)
		for (auto existingState : this->states)
		{
			if (existingState->getType() == state->getType())
			{
				ke::Log::instance()->error("An end state instance of {} already exists in this state machine.", state->getName());
			}
		}
#endif
        this->states.insert(state);
		this->endState = state.get();
    }

    bool StateMachine::addStateTransition(StateMachineStateType exitStateType, StateMachineStateExitCodeType exitStateExitCode, StateMachineStateType startStateType)
    {
		auto it_exitState = std::find_if(this->states.cbegin(), this->states.cend(),
								[exitStateType](ke::IStateMachine::StateSptr state) { return state->getType() == exitStateType; });
		if (it_exitState == this->states.cend())
		{
#if defined(KE_DEBUG)
			ke::Log::instance()->warn("The state machine does not contain a state of type {} ", exitStateExitCode);
#endif
			return false;
		}
		auto it_startState = std::find_if(this->states.cbegin(), this->states.cend(),
								[startStateType](ke::IStateMachine::StateSptr state) { return state->getType() == startStateType; });
		if (it_startState == this->states.cend())
		{
#if defined(KE_DEBUG)
			ke::Log::instance()->warn("The state machine does not contain a state of type {} ", startStateType);
#endif
			return false;
		}
		stateTransitionMap[exitStateExitCode][exitStateExitCode] = (*it_startState).get();
        return true;
    }

	IStateMachine::IState * StateMachine::getCurrentState()
	{
		return this->currentState;
	}

}