#include "KEngine/Core/Entity.hpp"
#include "KEngine/Interface/IEntityComponent.hpp"

#include <cassert>

namespace ke
{

    Entity::Entity(const ke::EntityId p_ID)
        : m_EntityID(p_ID)
    {}

    Entity::Entity(ke::Entity && p_rrEntity)
        : m_ComponentSPMap(std::move(p_rrEntity.m_ComponentSPMap))
        , m_EntityID(std::move(p_rrEntity.getId()))
    {}

    Entity & Entity::operator=(ke::Entity && p_rrEntity)
    {
        m_ComponentSPMap = std::move(p_rrEntity.m_ComponentSPMap);
        m_EntityID = std::move(p_rrEntity.getId());
        return *this;
    }

    Entity::~Entity(void)
    {
        assert(m_ComponentSPMap.empty());
    }

    void Entity::addComponent(ke::EntityComponentSptr p_spEntityComponent)
    {
        assert(p_spEntityComponent != nullptr); // should not be null.
        const auto result = m_ComponentSPMap.insert(std::make_pair(p_spEntityComponent->getId(), p_spEntityComponent));
        assert(result.second); // fails if insertion failed.
    }

    void Entity::updateAll(const ke::Time p_ElapsedTime)
    {
        for (auto & it : m_ComponentSPMap)  it.second->update(p_ElapsedTime);
    }

}