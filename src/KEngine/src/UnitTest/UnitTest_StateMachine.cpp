#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/StateMachine.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{

    class TestStateA : public ke::IStateMachine::IState
    {
    public:
    };

}


TEST_CASE("ke::StateMachine Unit Tests")
{

    ke::StateMachine sm;

    SECTION("")
    {
    }

}

#endif