#pragma once

#include "KEngine/Common/String.hpp"
#include "KEngine/Interface/IEntity.hpp"

namespace ke
{
    using EntityComponentId = std::uint64_t;

    using EntityComponentType = std::uint32_t;
    static const EntityComponentType INVALID_ENTITY_COMPONENT_TYPE = 0;


    /// <summary>
    /// A base class for implementing custom EntityComponent's. A EntityComponent is a container of 'data' belonging to an Entity.
    /// </summary>
    /// 
    /// An EntityComponent must be created with a valid Entity object. A valid Entity object must always
    /// be provided into the constructors. Bad things would happen if you don't!!!
    /// 
    /// Each different EntityComponent has a type, which is created from an UUID.
    /// Copy and move operations are disabled since this class utilises polymorphism.
    /// 
    class IEntityComponent
    {
        friend class EntityFactory;

    public:
        using IDType = ke::EntityComponentId; /// type of the component instance id. each instance has an unique id at run time.

        static const EntityComponentType TYPE = INVALID_ENTITY_COMPONENT_TYPE; // type is different to id.

        IEntityComponent(EntitySptr p_spEntity);
        IEntityComponent(const IEntityComponent &) = delete;
        //IEntityComponent(IEntityComponent &&) = delete; //NOTE: default move ctor not supported in VS12
        IEntityComponent & operator=(const IEntityComponent &) = delete;
        //IEntityComponent & operator=(const IEntityComponent &&) = delete; // NOTE: default move assigment operaot not supported in VS12
        virtual ~IEntityComponent(void) = 0;

        /// <summary>
        /// Initalise the component with a JSON config resource file.
        /// </summary>
        /// <returns>true on success.</returns>
        virtual bool initialise(void /* TODO: path to ini file? */) = 0;

        /// <summary>
        /// Carry out any post initialisation operations(e.g. operations that can't be done during initialisation).
        /// </summary>
        virtual void postInitialise(void) = 0;
        
        /// <summary>
        /// Update this EntityComponent instance.
        /// </summary>
        /// <param name="p_ElapsedTime"></param>
        virtual void update(const ke::Time p_ElapsedTime) = 0;
        
        /// <summary>
        /// Get the instance ID of this EntityComponent.
        /// </summary>
        /// <returns>the instance ID of this EntityComponent.</returns>
        virtual ke::EntityComponentId getId(void) const = 0;

        /// <summary>
        /// Get the type of this particular EntityComponent.
        /// </summary>
        /// <returns>the type of this particular EntityComponent.</returns>
        virtual EntityComponentType getType(void) const = 0;

        /// <summary>
        /// Get the name of this particular EntityComponent.
        /// </summary>
        /// <returns>the name of this particular EntityComponent.</returns>
        virtual const ke::String & getName(void) const = 0;

        /// <summary>
        /// Get a shared pointer to this Component's owner Entity.
        /// </summary>
        /// <returns>a shared pointer to this Component's owner Entity.</returns>
        EntitySptr getOwnerEntity(void);
        
        /// <summary>
        /// Set the owner Entity of this Component instance. Remove the component from it's original owner as well.
        /// </summary>
        /// <param name="p_spEntity"></param>
        void setOwnerEntity(ke::EntitySptr p_spEntity);

    protected:
        ke::EntitySptr  m_spOwnerEntity;  // owner
        ke::String      m_Name;

    }; // IEntityComponent class

    inline IEntityComponent::~IEntityComponent() {}
    inline const ke::String & IEntityComponent::getName(void) const { return m_Name; }
    inline EntityComponentType IEntityComponent::getType(void) const { return IEntityComponent::TYPE; }

    template<typename ComponentT, typename... ArgTs>
    //inline EntityComponentSptr makeEntityComponent(ArgTs && ... p_rrArgs)
    inline EntityComponentSptr makeEntityComponent(ArgTs && ... p_rrArgs)
    {
        static_assert(std::is_convertible<ComponentT*, IEntityComponent*>::value, "Given type cannot convert to ke::IEntityComponent.");
        static_assert(std::is_base_of<IEntityComponent, ComponentT>::value, "Given type does not have IEntityComponent as base class.");
        return std::make_shared<ComponentT>(std::forward<ArgTs>(p_rrArgs)...);
    }
    
}