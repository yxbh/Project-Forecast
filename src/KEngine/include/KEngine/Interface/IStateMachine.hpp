#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/String.hpp"

#include <cassert>
#include <memory>

namespace ke
{
    using StateMachineStateType = std::uint32_t;
    using SMStateType = StateMachineStateType;
    static const SMStateType INVALID_STATE_MACHINE_STATE_TYPE = 0;

    using StateMachineStateExitCode = std::uint32_t;
    using SMStateExitCode = StateMachineStateExitCode;

    /// <summary>
    /// Interface for implementing a state machine.
    /// </summary>
    class IStateMachine
    {
    public:
        using StateType = SMStateType;
        class IState;
        using StateUptr = std::unique_ptr<IStateMachine::IState>;
        using StateSptr = std::shared_ptr<IStateMachine::IState>;
        using StateWptr = std::weak_ptr<IStateMachine::IState>;


        IStateMachine() = default;
        IStateMachine(IStateMachine &) = delete;
        IStateMachine & operator = (const IStateMachine &) = delete;
        virtual ~IStateMachine() = 0;
        
        virtual bool initialise() = 0;
        virtual void shutdown() = 0;

        /// <summary>
        /// Update the state machine, which will in turn update the current state.
        /// When this function is called for the first time the state machine will automatically
        /// start from the start state.
        /// </summary>
        /// <param name="elapsedTime">The elapsed time since the last update.</param>
        virtual void update(const ke::Time & elapsedTime) = 0;

        virtual void finishState(IState * state, int stateExitCode) = 0;

        /// <summary>
        /// Add a state that neither the start state nor end state to the state machine.
        /// </summary>
        /// <param name="state"></param>
        virtual void addState(StateSptr state) = 0;
        /// <summary>
        /// Add the given state as the start state. This will replace any current start state.
        /// </summary>
        /// <param name="state"></param>
        virtual void addStartState(StateSptr state) = 0;
        /// <summary>
        /// Add the given state as the end state. This will replace any current end state.
        /// </summary>
        /// <param name="state"></param>
        virtual void addEndState(StateSptr state) = 0;
        /// <summary>
        /// Add a state transition that defines when a state of type exitStateType exits with the given
        /// exit code, the state machine will transition to the state of startStateType.
        /// This will replace any current transition that matches the given end state type and start state type.
        /// </summary>
        /// <param name="exitStateType">The type of the state that the exit will be mapped from.</param>
        /// <param name="exitStateExitCode">The type of the state that the enter will be mapped to.</param>
        /// <param name="startStateType"></param>
        virtual bool addStateTransition(StateMachineStateType exitStateType, StateMachineStateExitCode exitStateExitCode, StateMachineStateType startStateType) = 0;

        /// <summary>
        /// Interface for implementing state machine states.
        /// </summary>
        class IState
        {
        public:
            static const SMStateType TYPE = INVALID_STATE_MACHINE_STATE_TYPE;

            IState(IStateMachine * stateMachine) : stateMachine(stateMachine) { assert(stateMachine); }
            IState(IState &) = delete;
            IState & operator = (const IState &) = delete;
            virtual ~IState() = 0;

            /// <summary>
            /// This is a lifecycle method called by the owner state machine before entering the state.
            /// i.e. before update() is called for the first time.
            /// </summary>
            virtual void onEnter() = 0;

            /// <summary>
            /// This is a lifecycle method called by the owner state machine before exiting the state.
            /// i.e. after update() is called for the last time.
            /// </summary>
            virtual void onExit() = 0;

            inline void finish(int stateExitCode) { this->getStateMachine()->finishState(this, stateExitCode); }

            virtual void update(const ke::Time & elapsedTime) = 0;

            virtual SMStateType getType() const = 0;
            virtual const ke::String & getName() const = 0;

        protected:
            IStateMachine * stateMachine = nullptr;

            inline IStateMachine * getStateMachine() { return this->stateMachine; };

        }; // IState class


    }; // IStateMachine class

    inline IStateMachine::~IStateMachine() {}


    inline IStateMachine::IState::~IState() {}
    inline SMStateType IStateMachine::IState::getType() const { return IState::TYPE; }
    inline const ke::String &  IStateMachine::IState::getName() const { return KE_TEXT("ISMState"); }


    using IStateMachineUptr = std::unique_ptr<IStateMachine>;
    using IStateMachineSptr = std::shared_ptr<IStateMachine>;
    using IStateMachineWptr = std::weak_ptr<IStateMachine>;


}