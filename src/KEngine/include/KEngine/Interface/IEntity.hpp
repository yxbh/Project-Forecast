#pragma once

#include "KEngine/Common/String.hpp"
#include "KEngine/Common/Time.hpp"

#include <map>
#include <memory>
#include <vector>

namespace ke
{

    using EntityId = std::uint64_t;
    static const EntityId INVALID_ENTITY_ID = 0;
    

    class Entity;
    using EntitySptr = std::shared_ptr<ke::Entity>;
    using EntityWptr = std::weak_ptr<ke::Entity>;
    using EntityUptr = std::unique_ptr<ke::Entity>;
    using EntitySPMap = std::map<ke::EntityId, ke::EntitySptr>;


    using EntityComponentType = std::uint32_t;
    

    class IEntityComponent;
    using EntityComponentSptr = std::shared_ptr<ke::IEntityComponent>;
    using EntityComponentWptr = std::weak_ptr<ke::IEntityComponent>;
    using EntityComponentUptr = std::unique_ptr<ke::IEntityComponent>;


    using EntityComponentSPList = std::vector<ke::EntityComponentSptr>;
    using EntityComponentSPMap = std::map<ke::EntityComponentType, ke::EntityComponentSptr>;


    template<typename ... ArgTs>
    inline EntitySptr makeEntity(ArgTs && ... p_rrArgs)
    {
        return std::make_shared<ke::Entity>(std::forward<ArgTs>(p_rrArgs)...);
    }
    
}