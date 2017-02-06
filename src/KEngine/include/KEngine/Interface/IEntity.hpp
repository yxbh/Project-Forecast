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
    

    class IEntity;
    using EntitySptr = std::shared_ptr<ke::IEntity>;
    using EntityWptr = std::weak_ptr<ke::IEntity>;
    using EntityUptr = std::unique_ptr<ke::IEntity>;
    using EntitySPMap = std::map<ke::EntityId, ke::EntitySptr>;


    using EntityComponentType = std::uint32_t;
    

    class IEntityComponent;
    using EntityComponentSptr = std::shared_ptr<ke::IEntityComponent>;
    using EntityComponentWptr = std::weak_ptr<ke::IEntityComponent>;
    using EntityComponentUptr = std::unique_ptr<ke::IEntityComponent>;


    using EntityComponentSPList = std::vector<ke::EntityComponentSptr>;
    using EntityComponentSPMap = std::map<ke::EntityComponentType, ke::EntityComponentSptr>;


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
    class IEntity
    {
        friend class EntityFactory;

    public:
        using EntityType = ke::String;
        using TagList = std::vector<ke::String>;

    public:
        IEntity() = default;
        IEntity(const IEntity &) = delete;
        IEntity & operator = (const IEntity &) = delete;
        virtual ~IEntity(void) = 0;

        /// <summary>
        /// Adds a componenet. This function must be called when the Entity is passed to say EntityComponent.
        /// </summary>
        /// <param name="p_spEntityComponent"></param>
        virtual void addComponent(ke::EntityComponentSptr p_spEntityComponent) = 0;
        
        /// <summary>
        /// Destory the Entity. Breaks the circular/strong references to the Entity's components.
        /// </summary>
        virtual void destory(void) = 0;

        /// <summary>
        /// Update all components with elapsed time.
        /// </summary>
        /// <param name="p_ElapsedTime"></param>
        virtual void updateAll(const ke::Time p_ElapsedTime) = 0;

        /// <summary>
        /// Get the unique EntityID.
        /// </summary>
        /// <returns>the unique EntityID.</returns>
        virtual ke::EntityId getId(void) const = 0;

        /// /// <summary>
        /// Get the name of the entity.
        /// </summary>
        /// <returns>the name of the entity.</returns>
        virtual const std::string & getName(void) const = 0;

        /// <summary>
        /// Get a reference to the list of tags this Entity has.
        /// </summary>
        /// <returns>a reference to the list of tags this Entity has.</returns>
        virtual const TagList & getTags(void) const = 0;

        /// <summary>
        /// Get a component from the entity of the specified type.
        /// </summary>
        /// <param name="p_ComponentType"></param>
        /// <returns>a weak pointer to the requested Component type with the specified EntityComponentType.</returns>
        template <class ComponentClassType>
        std::weak_ptr<ComponentClassType> getComponent(const ke::EntityComponentType p_ComponentType);

    }; // IEntity class

    inline IEntity::~IEntity() {}


    template <class ComponentClassType>
    inline std::weak_ptr<ComponentClassType> IEntity::getComponent(const ke::EntityComponentType p_ComponentType)
    {
        EntityComponentSPMap::iterator it(m_ComponentSPMap.find(p_ComponentType));
        if (it == m_ComponentSPMap.end()) // does not contain component
            return nullptr;
        return std::weak_ptr<ComponentClassType>(std::static_pointer_cast<ComponentClassType>(it->second)); // return weak pointer version.
    }

    template<typename ... ArgTs>
    inline EntitySptr makeEntity(ArgTs && ... p_rrArgs)
    {
        return std::make_shared<ke::Entity>(std::forward<ArgTs>(p_rrArgs)...);
    }
    
}