#pragma once

#include "KEngine/Interface/IStateMachine.hpp"

#include <unordered_map>
#include <unordered_set>

namespace ke
{

    /// <summary>
    /// Default implementation of the ke::IStateMachine interface.
    /// </summary>
    class StateMachine : public IStateMachine
    {
    public:
        using StateTransitionMapType = std::unordered_map< StateMachineStateType, std::unordered_map<StateMachineStateExitCode, IState *>>;

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
        /// </summary>
        /// <param name="elapsedTime">The elapsed time since the last update.</param>
        virtual void update(const ke::Time & elapsedTime = ke::Time::Zero) override;

        /// <summary>
        /// Call this from a current state inside the state machine to request exiting the state.
        /// </summary>
        /// <param name="stateType"></param>
        /// <param name="stateExitCode"></param>
        virtual void finishState(IState * state, int stateExitCode) override;

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
        virtual bool addStateTransition(StateMachineStateType exitStateType, StateMachineStateExitCode exitStateExitCode, StateMachineStateType startStateType) override;

    private:

        std::unordered_set<StateSptr> states;
        IState * startState;
        IState * endState;
        IState * currentState;

        StateTransitionMapType stateTransitionMap;

    };

    inline StateMachine::~StateMachine() {}

    
}