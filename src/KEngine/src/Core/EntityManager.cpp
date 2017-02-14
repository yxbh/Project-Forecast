#include "KEngine/Core/EntityManager.hpp"

#include "KEngine/Log/Log.hpp"

namespace ke
{
    void EntityManager::update(ke::Time elapsedTime)
    {
        for (auto entityIdPair : this->entityMap)
        {
            entityIdPair.second->updateAll(elapsedTime);
        }
    }

    void EntityManager::removeEntity(ke::EntityId entityId)
    {
        auto entityIt = this->entityMap.find(entityId);
        if (entityIt != this->entityMap.end())
        {
            auto entity = entityIt->second;
            this->entityMap.erase(entityIt);
            entity->destory();
            entity.reset();
        }
        else
        {
#if defined(KE_DEBUG)
            ke::Log::instance()->warn("Entity with ID({}) not found.", entityId);
#endif
        }
    }

    ke::EntityWptr EntityManager::findEntityById(ke::EntityId entityId)
    {
        auto entityIt = this->entityMap.find(entityId);
        return entityIt == this->entityMap.end() ? nullptr : (*entityIt).second;
    }

}