#pragma once

#include "KEngine/Interface/IEntity.hpp"
#include "KEngine/Common/String.hpp"

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

        static constexpr const char * const NAME = "IEntityComponent";
        static const EntityComponentType TYPE = INVALID_ENTITY_COMPONENT_TYPE; // type is different to id.

        IEntityComponent(EntitySptr p_spEntity);
        IEntityComponent(const IEntityComponent &) = delete;
        IEntityComponent(IEntityComponent &&) = default;
        IEntityComponent & operator=(const IEntityComponent &) = delete;
        IEntityComponent & operator=(IEntityComponent &&) = default;
        virtual ~IEntityComponent(void) = 0;

        /// <summary>
        /// Initalise the component with a JSON config resource file.
        /// This member function is called by the EntityFactory at the time the component is created.
        /// </summary>
        /// <returns>true on success.</returns>
        virtual bool initialise(void) { return true; };

        /// <summary>
        /// Carry out any post initialisation operations(e.g. operations that can't be done during initialisation).
        /// </summary>
        virtual void postInitialise(void) {};
        
        /// <summary>
        /// Update this EntityComponent instance.
        /// </summary>
        /// <param name="p_ElapsedTime"></param>
        virtual void update(const ke::Time p_ElapsedTime) { KE_UNUSED(p_ElapsedTime); };

        /// <summary>
        /// Get the type of this particular EntityComponent.
        /// </summary>
        /// <returns>the type of this particular EntityComponent.</returns>
        virtual EntityComponentType getType(void) const = 0;

        /// <summary>
        /// Get the name of this particular EntityComponent.
        /// </summary>
        /// <returns>the name of this particular EntityComponent.</returns>
        virtual const char * getName(void) const = 0;

        /// <summary>
        /// Get a shared pointer to this Component's owner Entity.
        /// </summary>
        /// <returns>a shared pointer to this Component's owner Entity.</returns>
        inline EntitySptr getOwnerEntity(void)
        {
            return this->m_spOwnerEntity;
        }
        
        /// <summary>
        /// Set the owner Entity of this Component instance. Remove the component from it's original owner as well.
        /// </summary>
        /// <param name="p_spEntity"></param>
        inline void setOwnerEntity(ke::EntitySptr p_spEntity)
        {
            m_spOwnerEntity = p_spEntity;
        }

    protected:
        ke::EntitySptr m_spOwnerEntity;  // owner

    }; // IEntityComponent class

    inline IEntityComponent::IEntityComponent(EntitySptr p_spEntity) : m_spOwnerEntity(p_spEntity) {}
    inline IEntityComponent::~IEntityComponent() {}
    inline const char * IEntityComponent::getName(void) const { return IEntityComponent::NAME; }
    inline EntityComponentType IEntityComponent::getType(void) const { return IEntityComponent::TYPE; }

    template<typename ComponentT, typename... ArgTs>
    inline std::shared_ptr<ComponentT> makeEntityComponent(ArgTs && ... p_rrArgs)
    {
        static_assert(std::is_convertible<ComponentT*, IEntityComponent*>::value, "Given type cannot convert to ke::IEntityComponent.");
        static_assert(std::is_base_of<IEntityComponent, ComponentT>::value, "Given type does not have IEntityComponent as base class.");
        return std::make_shared<ComponentT>(std::forward<ArgTs>(p_rrArgs)...);
    }
    
}

#define KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(CLASS_NAME, COMPONENT_TYPE_VALUE) \
public: \
    static constexpr ke::EntityComponentType TYPE = COMPONENT_TYPE_VALUE; \
    static constexpr const char * const NAME = #CLASS_NAME; \
    virtual ke::EntityComponentType getType() const override { return CLASS_NAME::TYPE; } \
    virtual const char * getName() const override { return CLASS_NAME::NAME; } \
private:
