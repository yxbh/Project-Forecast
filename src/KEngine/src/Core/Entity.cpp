#include "KEngine/Core/Entity.hpp"
#include "KEngine/Interface/IEntityComponent.hpp"

#include <cassert>
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

    ke::EntityId Entity::newId()
    {
        auto & generator = ke::priv::getEntityIdGeneratorInstance();
        std::remove_reference<decltype(generator)>::type::result_type temp = generator();
        while (temp == INVALID_ENTITY_ID) temp = generator();
        return temp;
    }

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
        m_EntityID = p_rrEntity.getId();
        return *this;
    }

    Entity::~Entity(void)
    {
        assert(m_ComponentSPMap.empty());
    }

    void Entity::addComponent(ke::EntityComponentSptr p_spEntityComponent)
    {
        assert(p_spEntityComponent != nullptr); // should not be null.
        const auto result = m_ComponentSPMap.insert(std::make_pair(p_spEntityComponent->getType(), p_spEntityComponent));
        assert(result.second); // fails if insertion failed.
    }

    void Entity::updateAll(const ke::Time & p_ElapsedTime)
    {
        for (auto & it : m_ComponentSPMap)  it.second->update(p_ElapsedTime);
    }

}