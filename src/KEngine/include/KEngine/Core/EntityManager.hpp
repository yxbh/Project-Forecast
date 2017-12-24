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
        /// <summary>
        /// Update all entities.
        /// </summary>
        /// <param name="elapsedTime"></param>
        void update(ke::Time elapsedTime);

        /// <summary>
        /// Return a usable Entity object that is currently managed by the EntityManager.
        /// </summary>
        /// <returns></returns>
        ke::EntityWptr newEntity();

        /// <summary>
        /// Add the given entity to the EntityManager.
        /// This passes ownership of the entity to the manager.
        /// </summary>
        /// <param name="entity"></param>
        inline void addEntity(ke::EntitySptr entity) { this->entityMap.insert({ entity->getId(), entity }); }

        void removeEntity(ke::EntityId entityId);
        ke::EntityWptr findEntityById(ke::EntityId entityId);

    private:
        std::unordered_map<ke::EntityId, ke::EntitySptr> entityMap;
    };

}