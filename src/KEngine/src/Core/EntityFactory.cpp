#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Core/EntityFactory.hpp"

#include "KEngine/Log/Log.hpp"

#include <algorithm>

namespace
{
    static const int JSON_SPACE_INDENTATION = 2;
}

namespace ke
{

    EntityFactory::EntityFactory()
    {
        // register internal KEngine loaders here.
    }

    bool EntityFactory::registerComponentJsonLoader(const ke::String & entityComponentName, EntityComponentJsonLoaderSptr loader)
    {
        bool isLoaderForNameAlreadyExists = this->creators.find(entityComponentName) == this->creators.end();
        
        if (isLoaderForNameAlreadyExists)
        {
            ke::Log::instance()->warn("Json loader for component name {} already registered. It will be overrided.", entityComponentName);
        }

        this->creators[entityComponentName] = loader;

        return isLoaderForNameAlreadyExists;
    }

    ke::EntitySptr EntityFactory::createNew(const ke::Json & entityJsonObject) const
    {
        //auto jsonObjectTypeIt = entityJsonObject.find("type");
        //if (jsonObjectTypeIt == entityJsonObject.end())
        //{
        //    Log::instance()->error("Invalid JSON object received by EntityFactory::createNew. Content: {}", entityJsonObject.dump(JSON_SPACE_INDENTATION));
        //    return nullptr;
        //}

        //auto typeValue = (*jsonObjectTypeIt).get<std::string>();
        //if (typeValue != "entity")
        //{
        //    Log::instance()->error("Invalid JSON object type received by EntityFactory::createNew. Type given: {}", typeValue);
        //    return nullptr;
        //}

        return nullptr;
    }

}
