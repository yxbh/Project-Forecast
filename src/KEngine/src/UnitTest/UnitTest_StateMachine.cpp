#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/StateMachine.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{
    static bool testStartStateOnEnterCalled = false;
    static bool testStartStateOnExitCalled = false;

    static bool testStateAOnEnterCalled = false;
    static bool testStateAOnExitCalled = false;

    static bool testStateBOnEnterCalled = false;
    static bool testStateBOnExitCalled = false;

    static bool testEndStateOnEnterCalled = false;
    static bool testEndStateOnExitCalled = false;

    static void resetTestFlags()
    {
        testStartStateOnEnterCalled = false;
        testStartStateOnExitCalled = false;

        testStateAOnEnterCalled = false;
        testStateAOnExitCalled = false;

        testStateBOnEnterCalled = false;
        testStateBOnExitCalled = false;

        testEndStateOnEnterCalled = false;
        testEndStateOnExitCalled = false;
    }


	class TestStartState : public ke::IStateMachine::IState
	{
        KE_DEFINE_STATE_MACHINE_STATE_COMMON_PROPERTIES(TestStartState)
	public:
		static const ke::SMStateType TYPE = 1;

		static const ke::SMStateExitCodeType STATE_EXIT_SUCCESS = 1;

		using IState::IState;

		virtual void onEnter() override
        {
            testStartStateOnEnterCalled = true;
        }
		virtual void onExit() override
        {
            testStartStateOnExitCalled = true;
        }
		virtual void update(const ke::Time & elapsedTime) override
		{
			KE_UNUSED(elapsedTime);

			if (counter) this->finish(STATE_EXIT_SUCCESS);

			++counter;
		}

	private:
		int counter = 0;
	};

    class TestStateA : public ke::IStateMachine::IState
    {
        KE_DEFINE_STATE_MACHINE_STATE_COMMON_PROPERTIES(TestStateA)
    public:
		static const ke::SMStateType TYPE = 2;

		static const ke::SMStateExitCodeType STATE_EXIT_SUCCESS = 1;

		using IState::IState;

        virtual void onEnter() override
        {
            testStateAOnEnterCalled = true;
        }
        virtual void onExit() override
        {
            testStateAOnExitCalled = true;
        }
		virtual void update(const ke::Time & elapsedTime) override
        {
            KE_UNUSED(elapsedTime);
            this->finish(STATE_EXIT_SUCCESS);
        }
    };

	class TestStateB : public ke::IStateMachine::IState
	{
        KE_DEFINE_STATE_MACHINE_STATE_COMMON_PROPERTIES(TestStateB)
	public:
		static const ke::SMStateType TYPE = 3;

		static const ke::SMStateExitCodeType STATE_EXIT_SUCCESS = 1;

		using IState::IState;

        virtual void onEnter() override
        {
            testStateBOnEnterCalled = true;
        }
        virtual void onExit() override
        {
            testStateBOnExitCalled = true;
        }
		virtual void update(const ke::Time & elapsedTime) override { KE_UNUSED(elapsedTime); }
	};

	class TestEndState : public ke::IStateMachine::IState
	{
        KE_DEFINE_STATE_MACHINE_STATE_COMMON_PROPERTIES(TestEndState)
	public:
		static const ke::SMStateType TYPE = 4;

		static const ke::SMStateExitCodeType STATE_EXIT_SUCCESS = 1;

		using IState::IState;

        virtual void onEnter() override
        {
            testEndStateOnEnterCalled = true;
        }
        virtual void onExit() override
        {
            testEndStateOnExitCalled = true;
        }
		virtual void update(const ke::Time & elapsedTime) override
        {
            KE_UNUSED(elapsedTime);
            this->finish(STATE_EXIT_SUCCESS);
        }
	};

}

TEST_CASE("ke::StateMachine Unit Tests", "[kengine][core]")
{
    SECTION("Add state transition without adding states")
    {
        ke::StateMachine sm;
        CHECK(!sm.addStateTransition(TestStateA::TYPE, TestStateA::STATE_EXIT_SUCCESS, TestStateB::TYPE)); // existing states so this should return false.
    }
}

TEST_CASE("ke::StateMachine life cycles tests", "[kengine][core]")
{
    ke::StateMachine sm;
    CHECK(sm.getCurrentState() == nullptr);
    CHECK(sm.getStatus() == ke::StateMachine::Status::Finished);

    auto stateStart = ke::makeStateMachineState<TestStartState>(&sm);
    auto stateA = ke::makeStateMachineState<TestStateA>(&sm);
    auto stateB = ke::makeStateMachineState<TestStateB>(&sm);
    auto stateEnd = ke::makeStateMachineState<TestEndState>(&sm);

    SECTION("life cycles tests")
    {
        // the state machine and states are just constructed
        // no life cycle methods are called on the states
        CHECK(!testStartStateOnEnterCalled);
        CHECK(!testStartStateOnExitCalled);
        CHECK(!testStateAOnEnterCalled);
        CHECK(!testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(!testEndStateOnEnterCalled);
        CHECK(!testEndStateOnExitCalled);

        // adding the states to the state machine
        sm.addStartState(stateStart);
        sm.addState(stateA);
        sm.addState(stateB);
        sm.addEndState(stateEnd);

        // no life cycle methods are called on the states
        CHECK(!testStartStateOnEnterCalled);
        CHECK(!testStartStateOnExitCalled);
        CHECK(!testStateAOnEnterCalled);
        CHECK(!testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(!testEndStateOnEnterCalled);
        CHECK(!testEndStateOnExitCalled);

        CHECK(sm.getStatus() == ke::StateMachine::Status::Finished);

        // state transitions are added to the state machine
        CHECK(sm.addStateTransition(TestStartState::TYPE, TestStartState::STATE_EXIT_SUCCESS, TestStateA::TYPE));

        // no life cycle methods are called on the states
        CHECK(!testStartStateOnEnterCalled);
        CHECK(!testStartStateOnExitCalled);
        CHECK(!testStateAOnEnterCalled);
        CHECK(!testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(!testEndStateOnEnterCalled);
        CHECK(!testEndStateOnExitCalled);

        CHECK(sm.getStatus() == ke::StateMachine::Status::Finished);

        // update() is called on the state machine for the 1st time
        // TestStartState's counter variable will increment to 1.
        // next time it is updated it will request exit.
        sm.update();
        CHECK(sm.getStatus() == ke::StateMachine::Status::Running);

        // the state machine's current state should be changed to TestStartState
        CHECK(sm.getCurrentState() == stateStart.get());
        CHECK(sm.getCurrentState()->getType() == stateStart.get()->getType());
        CHECK(sm.getCurrentState()->getName() == stateStart.get()->getName());

        CHECK(testStartStateOnEnterCalled);
        CHECK(!testStartStateOnExitCalled);
        CHECK(!testStateAOnEnterCalled);
        CHECK(!testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(!testEndStateOnEnterCalled);
        CHECK(!testEndStateOnExitCalled);


        // update() is called on the state machine again (2nd time)
        // TestStartState will have requested exit and incremented its counter variable 2. 
        // the state machine will have transitioned into TestStateA.
        sm.update();
        CHECK(sm.getStatus() == ke::StateMachine::Status::Running);

        // the state machine's current state should be changed to TestStateA
        CHECK(sm.getCurrentState() == stateA.get());
        CHECK(sm.getCurrentState()->getType() == stateA.get()->getType());
        CHECK(sm.getCurrentState()->getName() == stateA.get()->getName());

        CHECK(testStartStateOnEnterCalled);
        CHECK(testStartStateOnExitCalled);
        CHECK(testStateAOnEnterCalled);
        CHECK(!testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(!testEndStateOnEnterCalled);
        CHECK(!testEndStateOnExitCalled);

        // update() is called on the state machine again (3rd time)
        // State machine would want to transition to the next state but because
        // there's no transition given that maps from TestStateA, the state machine
        // will transition directly to TestEndState. TestStateB is ignored.
        sm.update();
        CHECK(sm.getStatus() == ke::StateMachine::Status::Running);

        // the state machine's current state should be changed to TestEndState
        CHECK(sm.getCurrentState() == stateEnd.get());
        CHECK(sm.getCurrentState()->getType() == stateEnd.get()->getType());
        CHECK(sm.getCurrentState()->getName() == stateEnd.get()->getName());

        CHECK(testStartStateOnEnterCalled);
        CHECK(testStartStateOnExitCalled);
        CHECK(testStateAOnEnterCalled);
        CHECK(testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(testEndStateOnEnterCalled);
        CHECK(!testEndStateOnExitCalled);

        // update() is called on the state machine again (4th time)
        // TestEndState will request exit.
        // The state machine will reset to initial state.
        sm.update();
        CHECK(sm.getStatus() == ke::StateMachine::Status::Finished);

        CHECK(sm.getCurrentState() == nullptr);

        CHECK(testStartStateOnEnterCalled);
        CHECK(testStartStateOnExitCalled);
        CHECK(testStateAOnEnterCalled);
        CHECK(testStateAOnExitCalled);
        CHECK(!testStateBOnEnterCalled);
        CHECK(!testStateBOnExitCalled);
        CHECK(testEndStateOnEnterCalled);
        CHECK(testEndStateOnExitCalled);
    }

    resetTestFlags();

}

#endif