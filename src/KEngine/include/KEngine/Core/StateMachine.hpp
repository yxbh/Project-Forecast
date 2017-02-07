#pragma once

#include "KEngine/Interface/IStateMachine.hpp"

#include <vector>

namespace ke
{

    /// <summary>
    /// Default implementation of the ke::IStateMachine interface.
    /// </summary>
    class StateMachine : public IStateMachine
    {
    public:
        virtual ~StateMachine();

        virtual bool initialise() override;
        virtual void shutdown() override;

        /// <summary>
        /// 
        /// </summary>
        virtual void enter() override;
        /// <summary>
        /// Call this from a running state to exit the current state.
        /// </summary>
        virtual void exit() override;

        /// <summary>
        /// Start the state machine.
        /// </summary>
        virtual void begin() override;

        /// <summary>
        /// Update the state machine. If the specified time span is 0, the state machine or its states may update with its internal timer.
        /// </summary>
        /// <param name="elapsedTime">The elapsed time since the last update.</param>
        virtual void update(const ke::Time & elapsedTime = ke::Time::Zero) override;

    private:
        std::vector<ISMStateUptr> states;
        ISMState * currentState;

    };

    inline StateMachine::~StateMachine() {}

    
}