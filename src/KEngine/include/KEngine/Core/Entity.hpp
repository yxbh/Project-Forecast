#pragma once

#include "KEngine/Interface/IEntity.hpp"

namespace ke
{

    /// <summary>
    /// A container and manager class for EntityComponents. Also an abstract representation of an entity.
    /// </summary>
    /// 
    /// This class is not meant to be inhertied.
    /// This class is not copyable.
    /// 
    /// Entity holds strong reference to it's Components and the components also hold strong references to
    /// the entity.Therefore Destory() must be called before destorying an entity instance otherwise the
    /// Entity cannnot be destoryed due to circular reference from the entity components, and the components
    /// won't be able to destory themselves either.
    /// 
    /// This class is modeled after GCC(4th ed)'s Actor class, and so the principles are similar.
    class Entity final : public ke::IEntity
    {
        friend class EntityFactory;

    public:
        Entity(const ke::EntityId = INVALID_ENTITY_ID);
        Entity(ke::Entity && p_rrEntity);
        Entity & operator=(ke::Entity && p_rrEntity);
        virtual ~Entity(void);

        /// <summary>
        /// Adds a componenet. This function must be called when the Entity is passed to say EntityComponent.
        /// </summary>
        /// <param name="p_spEntityComponent"></param>
        void addComponent(ke::EntityComponentSptr p_spEntityComponent);

        /// <summary>
        /// Destory the Entity. Breaks the circular/strong references to the Entity's components.
        /// </summary>
        inline void destory(void) { m_ComponentSPMap.clear(); }

        /// <summary>
        /// Update all components with elapsed time.
        /// </summary>
        /// <param name="p_ElapsedTime"></param>
        virtual void updateAll(const ke::Time p_ElapsedTime);

        /// <summary>
        /// Get the unique EntityID.
        /// </summary>
        /// <returns>the unique EntityID.</returns>
        virtual ke::EntityId getId(void) const final;

        /// <summary>
        /// Get the name of the entity.
        /// </summary>
        /// <returns>the name of the entity.</returns>
        virtual const ke::String & getName(void) const final;

        /// <summary>
        /// Get a reference to the list of tags this Entity has.
        /// </summary>
        /// <returns>a reference to the list of tags this Entity has.</returns>
        virtual const TagList & getTags(void) const final;

        /// <summary>
        /// Get a component from the entity of the specified type.
        /// </summary>
        /// <param name="p_ComponentType"></param>
        /// <returns>a weak pointer to the requested Component type with the specified EntityComponentType.</returns>
        template <class ComponentClassType>
        std::weak_ptr<ComponentClassType> getComponent(const ke::EntityComponentType p_ComponentType);

    private:
        ke::EntityComponentSPMap        m_ComponentSPMap; // <EntityComponentType, EntityComponent>
        ke::EntityId                    m_EntityID;

        TagList                         m_Tags; /** */
        ke::String                      m_Name; /** An usually unique name. */

    }; // Entity class

    inline ke::EntityId Entity::getId(void) const { return m_EntityID; }
    inline const Entity::TagList & Entity::getTags(void) const { return m_Tags; }
    inline const std::string & Entity::getName(void) const { return m_Name; }


    template <class ComponentClassType>
    inline std::weak_ptr<ComponentClassType> Entity::getComponent(const ke::EntityComponentType p_ComponentType)
    {
        EntityComponentSPMap::iterator it(m_ComponentSPMap.find(p_ComponentType));
        if (it == m_ComponentSPMap.end()) // does not contain component
            return nullptr;
        return std::weak_ptr<ComponentClassType>(std::static_pointer_cast<ComponentClassType>(it->second)); // return weak pointer version.
    }

}