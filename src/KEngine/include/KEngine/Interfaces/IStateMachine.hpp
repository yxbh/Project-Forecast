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

    using StateMachineStateExitCodeType = std::int32_t;
    using SMStateExitCodeType = StateMachineStateExitCodeType;

    /// <summary>
    /// Interface for implementing a state machine.
    /// </summary>
	///
	/// A state machine is a container of states and it manages the lifecycle of each state and their transition.
	///
    class IStateMachine
    {
    public:
        using StateType = SMStateType;
        class IState;
        using StateUptr = std::unique_ptr<IStateMachine::IState>;
        using StateSptr = std::shared_ptr<IStateMachine::IState>;
        using StateWptr = std::weak_ptr<IStateMachine::IState>;

        enum class Status
        {
            Running,
            Finished
        };

        /// <summary>
        /// Interface for implementing state machine states.
        /// </summary>
        ///
        /// All child class must define a public static TYPE variable of type SMStateType.
        /// This value must be unique among all IState child classes.
        ///
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
            virtual ke::String getName() const = 0;

        protected:
            IStateMachine * stateMachine = nullptr;

            inline IStateMachine * getStateMachine() { return this->stateMachine; };

        }; // IState class


        IStateMachine() = default;
        IStateMachine(IStateMachine &) = delete;
        IStateMachine & operator = (const IStateMachine &) = delete;
        virtual ~IStateMachine() = 0;

        /// <summary>
        /// Update the state machine, which will in turn update the current state.
        /// When this function is called for the first time the state machine will automatically
        /// start from the start state.
        /// State transitions will also occur here.
        /// </summary>
        /// <param name="elapsedTime">The elapsed time since the last update.</param>
        virtual void update(const ke::Time & elapsedTime) = 0;

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
        virtual bool addStateTransition(StateMachineStateType exitStateType, StateMachineStateExitCodeType exitStateExitCode, StateMachineStateType startStateType) = 0;

		virtual IState * getCurrentState() = 0;

        /// <summary>
        /// Get the current running state.
        /// </summary>
        /// <returns>nullptr if the state machine is not running or has exited/reset.</returns>
        Status getStatus() const { return this->status; }

    protected:
        /// <summary>
        /// Call this from inside a current state inside the state machine to request exiting the state.
        /// </summary>
        /// <param name="state">the state that is requesting exit.</param>
        /// <param name="stateExitCode">the exit code of the state requesting exit.</param>
        virtual void finishState(IState * state, int stateExitCode) = 0;

        Status status = Status::Finished;


    }; // IStateMachine class

    inline IStateMachine::~IStateMachine() {}


    inline IStateMachine::IState::~IState() {}
    inline SMStateType IStateMachine::IState::getType() const { return IState::TYPE; }
    inline ke::String IStateMachine::IState::getName() const { return KE_TEXT("ISMState"); }


    using IStateMachineUptr = std::unique_ptr<IStateMachine>;
    using IStateMachineSptr = std::shared_ptr<IStateMachine>;
    using IStateMachineWptr = std::weak_ptr<IStateMachine>;

	template<typename State_T, typename ... Args_T >
	auto makeStateMachineState(Args_T && ... args ) -> IStateMachine::StateSptr
	{
		return std::make_shared<State_T>(std::forward<Args_T>(args)...);
	}
}


/// <summary>
/// Define the following common ke::IStateMachine::IState property member functions based on the class name:
/// * getType()
/// * getName()
/// </summary>
#define KE_DEFINE_STATE_MACHINE_STATE_COMMON_PROPERTIES(CLASS_NAME) \
public: \
	virtual ke::SMStateType getType() const override { return CLASS_NAME::TYPE; } \
	virtual ke::String getName() const override { return KE_TEXT(#CLASS_NAME); } \
private:
