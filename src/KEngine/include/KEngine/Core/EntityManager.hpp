#pragma once

#include "Entity.hpp"

#include <unordered_map>

namespace ke
{

    /// <summary>
    /// Default entity manager implementation.
    /// </summary>
    class EntityManager final
    {
    public:
        inline void addEntity(ke::EntitySptr entity) { this->entityMap.insert({ entity->getId(), entity }); }
        void removeEntity(ke::EntityId entityId);
        ke::EntityWptr findEntityById(ke::EntityId entityId);

    private:
        std::unordered_map<ke::EntityId, ke::EntitySptr> entityMap;
    };

}