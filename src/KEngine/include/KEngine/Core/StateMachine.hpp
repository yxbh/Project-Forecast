#pragma once

#include "KEngine/Interface/IStateMachine.hpp"

#include <unordered_map>
#include <unordered_set>

namespace ke
{

    /// <summary>
    /// Default implementation of the ke::IStateMachine interface.
    /// </summary>
	///
	/// A state machine is a container of states and it manages the lifecycle of each state and their transition.
	///
    class StateMachine : public IStateMachine
    {
    public:
        using StateTransitionMapType = std::unordered_map< StateMachineStateType, std::unordered_map<StateMachineStateExitCodeType, IState *>>;

        virtual ~StateMachine();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        virtual bool initialise() override;
        virtual void shutdown() override;

        /// <summary>
        /// Update the state machine, which will in turn update the current state.
        /// When this function is called for the first time the state machine will automatically
        /// start from the start state.
        /// State transitions will also occur here.
        /// </summary>
        /// <param name="elapsedTime">The elapsed time since the last update.</param>
        virtual void update(const ke::Time & elapsedTime = ke::Time::Zero) override;

        /// <summary>
        /// Add a state that neither the start state nor end state to the state machine.
        /// </summary>
        /// <param name="state"></param>
        virtual void addState(StateSptr state) override;
        /// <summary>
        /// Add the given state as the start state. This will replace any current start state.
        /// </summary>
        /// <param name="state"></param>
        virtual void addStartState(StateSptr state) override;
        /// <summary>
        /// Add the given state as the end state. This will replace any current end state.
        /// </summary>
        /// <param name="state"></param>
        virtual void addEndState(StateSptr state) override;
        /// <summary>
        /// Add a state transition that defines when a state of type exitStateType exits with the given
        /// exit code, the state machine will transition to the state of startStateType.
        /// This will replace any current transition that matches the given end state type and start state type.
        /// </summary>
        /// <param name="exitStateType">The type of the state that the exit will be mapped from.</param>
        /// <param name="exitStateExitCode">The type of the state that the enter will be mapped to.</param>
        /// <param name="startStateType"></param>
        /// <returns>true if successful. false if no matching state types are found.</returns>
        virtual bool addStateTransition(StateMachineStateType exitStateType, StateMachineStateExitCodeType exitStateExitCode, StateMachineStateType startStateType) override;
        
        /// <summary>
        /// Get the current running state.
        /// </summary>
        /// <returns>nullptr if the state machine is not running or has exited/reset.</returns>
		virtual IState * getCurrentState() override;

    protected:
        /// <summary>
        /// Call this from inside a current state inside the state machine to request exiting the state.
        /// </summary>
        /// <param name="state">the state that is requesting exit.</param>
        /// <param name="stateExitCode">the exit code of the state requesting exit.</param>
        virtual void finishState(IState * state, int stateExitCode) override;

    private:
        std::unordered_set<StateSptr> states;

        IState * startState = nullptr;
        IState * endState = nullptr;
        IState * currentState = nullptr;

        bool isCurrentStateExitRequested = false;
        SMStateExitCodeType currentStateExitCode = -1;

        StateTransitionMapType stateTransitionMap;

    };

    inline StateMachine::~StateMachine() {}

    
}