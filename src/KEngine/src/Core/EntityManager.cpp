#include "KEngine/Core/EntityManager.hpp"

#include "KEngine/Log/Log.hpp"

#include <chrono>
#include <random>
#include <type_traits>

namespace ke::priv
{
    static auto & getEntityIdGeneratorInstance()
    {
        static auto generator = std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count());
        return generator;
    }
}

namespace ke
{
    static auto logger = ke::Log::createDefaultLogger("EntityManager");

    void EntityManager::update(ke::Time elapsedTime)
    {
        for (auto & entityIdPair : this->entityMap)
        {
            entityIdPair.second->updateAll(elapsedTime);
        }
    }

    ke::EntityWptr EntityManager::newEntity(ke::EntityId newEntityId)
    {
        if (newEntityId != ke::INVALID_ENTITY_ID)
        {
            if (auto entity = this->findEntityById(newEntityId); !entity.expired())
            {
                logger->critical("The specified entity ID({}) for a new entity already exists in an existing entity.", newEntityId);
                assert(entity.expired());
            }
        }
        else
        {
            newEntityId = ke::EntityManager::newId();
        }
        auto entity = ke::makeEntity(newEntityId);
        this->addEntity(entity);
        return entity;
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
            logger->warn("Entity with ID({}) not found.", entityId);
#endif
        }
    }

    ke::EntityWptr EntityManager::findEntityById(ke::EntityId entityId)
    {
        auto entityIt = this->entityMap.find(entityId);
        return entityIt == this->entityMap.end() ? nullptr : (*entityIt).second;
    }

    const ke::EntityId EntityManager::newId(void) const
    {
        auto & generator = ke::priv::getEntityIdGeneratorInstance();
        std::remove_reference<decltype(generator)>::type::result_type temp = generator();
        while (temp == INVALID_ENTITY_ID && this->entityMap.find(temp) != this->entityMap.end()) temp = generator();
        return temp;
    }
}