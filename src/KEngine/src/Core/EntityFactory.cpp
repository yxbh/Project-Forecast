#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Core/Entity.hpp"

#include "KEngine/Log/Log.hpp"

#include <algorithm>

namespace ke
{

    EntityFactory::EntityFactory()
    {
        // register internal KEngine loaders here.
    }

    bool EntityFactory::registerEntityBuilder(const ke::String & p_entityTypeName, ke::EntityBuilderUptr && p_builder)
    {
        bool alreadyExists = this->entityBuilders.find(p_entityTypeName) != this->entityBuilders.end();

        if (alreadyExists)
        {
            ke::Log::instance()->warn("Loader for component named {} already registered. It will be overrided.", p_entityTypeName);
        }

        this->entityBuilders[p_entityTypeName] = std::move(p_builder);

        return !alreadyExists;
    }

    bool EntityFactory::registerComponentBuilder(const ke::String & p_entityComponentName, EntityComponentLoaderUptr && loader)
    {
        bool isLoaderForNameAlreadyExists = this->entityComponentBuilders.find(p_entityComponentName) != this->entityComponentBuilders.end();
        
        if (isLoaderForNameAlreadyExists)
        {
            ke::Log::instance()->warn("Loader for component named {} already registered. It will be overrided.", p_entityComponentName);
        }

        this->entityComponentBuilders[p_entityComponentName] = std::move(loader);

        return !isLoaderForNameAlreadyExists;
    }

    ke::Entity * EntityFactory::createNew(const ke::String & p_name, const std::any & p_parameters) const
    {
        if (auto itr = this->entityBuilders.find(p_name); itr != this->entityBuilders.end())
        {
            auto builder = itr->second.get();
            return builder->build(p_parameters);
        }
        ke::Log::instance()->error("EntityFactory does not have an entity builder for {}", p_name);
        return nullptr;
    }

    ke::IEntityComponentBuilder * EntityFactory::getComponentBuilder(const ke::String & p_componentName)
    {
        if (auto itr = this->entityComponentBuilders.find(p_componentName); itr != this->entityComponentBuilders.end())
        {
            return itr->second.get();
        }
        ke::Log::instance()->error("EntityFactory does not have an entity component builder for {}", p_componentName);
        return nullptr;
    }

}
