#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/ISystem.hpp"
#include "KEngine/Interfaces/IEntity.hpp"

#include <vector>

namespace pf
{

    /// <summary>
    /// 
    /// </summary>
    class DebugSceneGridSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(DebugSceneGridSystem, 0xd6cd4727)

    public:

        virtual bool initialise() final;
        virtual void shutdown() final;

        virtual void update(ke::Time elapsedTime) final;

        void handleRequest(ke::EventSptr event);

    private:
        std::vector<ke::EntityWptr> gridLineEntities;
    };


}