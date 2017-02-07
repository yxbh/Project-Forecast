#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/String.hpp"

#include <memory>

namespace ke
{
    using SMStateType = std::uint32_t;
    static const SMStateType INVALID_STATE_MACHINE_STATE_TYPE = 0;

    /// <summary>
    /// Interface for implementing a state machine.
    /// </summary>
    class IStateMachine
    {
    public:
        virtual ~IStateMachine() = 0;
        
        virtual bool initialise() = 0;
        virtual void shutdown() = 0;

        virtual void enter() = 0;
        virtual void exit() = 0;

        virtual void begin() = 0;
        virtual void update(const ke::Time & elapsedTime) = 0;
    };

    inline IStateMachine::~IStateMachine() {}

    /// <summary>
    /// Interface for implementing state machine states.
    /// </summary>
    class ISMState
    {
    public:
        static const SMStateType TYPE = INVALID_STATE_MACHINE_STATE_TYPE;

        ISMState(IStateMachine * stateMachine);
        virtual ~ISMState() = 0;

        virtual bool initialise() = 0;
        virtual void shutdown() = 0;

        virtual void enter() = 0;
        virtual void exit() = 0;

        virtual void update(const ke::Time & elapsedTime) = 0;

        virtual SMStateType getType() const = 0;
        virtual const ke::String & getName() const = 0;
    };

    inline ISMState::~ISMState() {}
    inline SMStateType ISMState::getType() const { return ISMState::TYPE; }
    inline const ke::String &  ISMState::getName() const { return KE_TEXT("ISMState"); }


    using IStateMachineUptr = std::unique_ptr<IStateMachine>;
    using IStateMachineSptr = std::shared_ptr<IStateMachine>;
    using IStateMachineWptr = std::weak_ptr<IStateMachine>;


    using ISMStateUptr = std::unique_ptr<ISMState>;
    using ISMStateSptr = std::shared_ptr<ISMState>;
    using ISMStateWptr = std::weak_ptr<ISMState>;

}