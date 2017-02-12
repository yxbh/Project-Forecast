#pragma once

#include "KEngine/Interface/IEntity.hpp"

#include <unordered_set>

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
    class Entity final
    {
        friend class EntityFactory;

    public:
        using TagList = std::unordered_set<ke::String>;

        /// <summary>
        /// Generate a new ID for uniquely identifying an Entity.
        /// </summary>
        /// <returns>a new EntityId.</returns>
        static ke::EntityId newId();

        Entity(const ke::EntityId = INVALID_ENTITY_ID);
        Entity(const Entity &) = delete;
        Entity(ke::Entity && p_rrEntity);
        Entity & operator = (const Entity &) = delete;
        Entity & operator=(ke::Entity && p_rrEntity);
        virtual ~Entity(void);

        /// <summary>
        /// Set the name of the Entity.
        /// </summary>
        /// <param name="name"></param>
        void setName(const ke::String & name);

        /// <summary>
        /// Add a tag to this Entity.
        /// </summary>
        /// <param name="tag"></param>
        void addTag(const ke::String & tag);

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
        void updateAll(const ke::Time & p_ElapsedTime);

        /// <summary>
        /// Get the unique EntityID.
        /// </summary>
        /// <returns>the unique EntityID.</returns>
        ke::EntityId getId(void) const;

        /// <summary>
        /// Get the name of the entity.
        /// </summary>
        /// <returns>the name of the entity.</returns>
        const ke::String & getName(void) const;

        /// <summary>
        /// Get a reference to the list of tags this Entity has.
        /// </summary>
        /// <returns>a reference to the list of tags this Entity has.</returns>
        const TagList & getTags(void) const;

        /// <summary>
        /// Get the number of components that are current held by the Entity.
        /// </summary>
        /// <returns>a value indidcating the number of compoonents that are held by the Entity.</returns>
        inline std::size_t getComponentCount(void) const { return m_ComponentSPMap.size(); }

        /// <summary>
        /// Check if this Entity has the specified tag.
        /// </summary>
        /// <param name="tag">the tag to search.</param>
        /// <returns>true if the Entity contains the specified tag.</returns>
        bool hasTag(const ke::String & tag) const;

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

    inline void Entity::setName(const ke::String & name) { m_Name = name; }
    inline void Entity::addTag(const ke::String & tag) { m_Tags.insert(tag); }

    inline ke::EntityId Entity::getId(void) const { return m_EntityID; }
    inline const Entity::TagList & Entity::getTags(void) const { return m_Tags; }
    inline const ke::String & Entity::getName(void) const { return m_Name; }

    inline bool Entity::hasTag(const ke::String & tag) const { return m_Tags.find(tag) != m_Tags.end(); }


    template <class ComponentClassType>
    inline std::weak_ptr<ComponentClassType> Entity::getComponent(const ke::EntityComponentType p_ComponentType)
    {
        EntityComponentSPMap::iterator it(m_ComponentSPMap.find(p_ComponentType));
        if (it == m_ComponentSPMap.end()) // does not contain component
            return nullptr;
        return std::weak_ptr<ComponentClassType>(std::static_pointer_cast<ComponentClassType>(it->second)); // return weak pointer version.
    }

}