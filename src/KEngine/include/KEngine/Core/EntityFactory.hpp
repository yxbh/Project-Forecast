#pragma once

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/IEntityBuilder.hpp"
#include "KEngine/Interfaces/IEntityComponentBuilder.hpp"

#include <any>
#include <unordered_map>

namespace ke
{

    class EntityFactory final
    {
    public:
        using EntityComponentCreatorMap = std::unordered_map<ke::String, EntityComponentLoaderUptr>;

        EntityFactory();

        /// <summary>
        /// Register and take ownership of the given entity builder object.
        /// This object is used when <see cref="createNew"/> is called.
        /// </summary>
        /// <param name="p_entityTypeName"></param>
        /// <param name="p_builder"></param>
        /// <returns>true if registration was successful.</returns>
        bool registerEntityBuilder(const ke::String & p_entityTypeName, ke::EntityBuilderUptr && p_builder);

        bool registerComponentBuilder(const ke::String & p_entityComponentName, EntityComponentLoaderUptr && loader);

        template<typename EntityComponentLoader_T>
        bool registerComponentBuilder(const ke::String & p_entityComponentName);

        /// <summary>
        /// Instantiate an instance of the given Entity type name with the given parameters.
        /// </summary>
        /// <param name="p_name">A type of entity to instantiate.</param>
        /// <param name="p_parameters">The parameters required to construct the named entity. This is passed to the respective builder and can be anything.</param>
        /// <returns></returns>
        ke::Entity * createNew(const ke::String & p_name, const std::any p_parameters) const;

        ke::IEntityComponentBuilder * getComponentBuilder(const ke::String & p_componentName);

        inline void clear() { this->entityComponentBuilders.clear(); this->entityBuilders.clear(); }

    private:
        EntityComponentCreatorMap entityComponentBuilders;

        std::unordered_map<ke::String, ke::EntityBuilderUptr> entityBuilders;
         
    };

    template<typename EntityComponentLoader_T>
    inline bool EntityFactory::registerComponentBuilder(const ke::String & p_entityComponentName)
    {
        return this->registerComponentBuilder(p_entityComponentName, std::make_unique<EntityComponentLoader_T>());
    }

}
