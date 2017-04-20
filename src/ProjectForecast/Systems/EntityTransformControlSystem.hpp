#pragma once

#include "KEngine/Interfaces/ISystem.hpp"
#include "KEngine/Interfaces/IEvent.hpp"

namespace pf
{
    
    class EntityTransformControlSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(EntityTransformControlSystem, 0xB4AAC4C2)

    public:

        virtual bool initialise() final;
        virtual void shutdown() final;

        virtual void update(ke::Time elapsedTime) final;

    private:
        void handleEntityTransformControlCommand(ke::EventSptr event);
        void handleEntityTransformDeltaControlCommand(ke::EventSptr event);
    };

}