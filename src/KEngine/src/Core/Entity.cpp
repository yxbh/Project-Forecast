#include "KEngine/Core/Entity.hpp"
#include "KEngine/Interfaces/IEntityComponent.hpp"
#include "KEngine/Log/Log.hpp"

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
        : m_Components(std::move(p_rrEntity.m_Components))
        , m_ComponentSPMap(std::move(p_rrEntity.m_ComponentSPMap))
        , m_EntityID(std::move(p_rrEntity.getId()))
    {}

    Entity & Entity::operator=(ke::Entity && p_rrEntity)
    {
        m_Components = std::move(p_rrEntity.m_Components);
        m_ComponentSPMap = std::move(p_rrEntity.m_ComponentSPMap);
        m_EntityID = p_rrEntity.getId();
        return *this;
    }

    Entity::~Entity(void)
    {
        assert(this->m_ComponentSPMap.empty()); // component must be empty (all circular references removed).
    }

    void Entity::addComponent(ke::EntityComponentSptr p_spEntityComponent)
    {
        assert(p_spEntityComponent != nullptr); // should not be null.
        const auto result = m_ComponentSPMap.insert(std::make_pair(p_spEntityComponent->getType(), p_spEntityComponent));
        assert(result.second); // fails if insertion failed.
        if (result.second)
        {
            ke::Log::instance()->error("Attempted to add entity component of duplicate type: {0:#x}", p_spEntityComponent->getType());
            return;
        }
        m_Components.push_back(p_spEntityComponent);
    }

    bool Entity::initialise(void)
    {
        bool result = true;
        for (auto & it : m_ComponentSPMap)
            if (!it.second->isInitialised())
            {
                if (it.second->initialise())
                {
                    it.second->postInitialise();
                }
                else
                {
                    ke::Log::instance()->error("Entity named '{}'({}) failed to initialise its component: '{}'",
                        this->getName(), this->getId(), it.second->getName());
                    result = false;
                }
            }
            
        return result;
    }

    void Entity::updateAll(const ke::Time & p_ElapsedTime)
    {
        if (!this->m_Initialised)
        {
            this->m_Initialised = this->initialise();
        }

        for (auto & it : m_Components)
            it->update(p_ElapsedTime);
    }

}