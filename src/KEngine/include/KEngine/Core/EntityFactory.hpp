#pragma once

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/IEntityComponentJsonLoader.hpp"

#include <unordered_map>

namespace ke
{

    class EntityFactory final
    {
    public:
        using EntityComponentCreatorMap = std::unordered_map<ke::String, EntityComponentJsonLoaderSptr>;

        EntityFactory();

        bool registerComponentJsonLoader(const ke::String & entityComponentName, EntityComponentJsonLoaderSptr loader);

        template<typename EntityComponentJsonLoader_T>
        bool registerComponentJsonLoader(const ke::String & entityComponentName);

        ke::EntitySptr createNew(const ke::Json & entityJsonObject) const;

        inline void clear() { this->creators.clear(); }

    private:
        EntityComponentCreatorMap creators;
         
    };

    template<typename EntityComponentJsonLoader_T>
    inline bool EntityFactory::registerComponentJsonLoader(const ke::String & entityComponentName)
    {
        return this->registerComponentJsonLoader(entityComponentName, std::make_shared<EntityComponentJsonLoader_T>());
    }

}
