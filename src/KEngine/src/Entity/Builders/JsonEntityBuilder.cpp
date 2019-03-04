#include "KEngine/Entity/Builders/JsonEntityBuilder.hpp"

#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Core/Entity.hpp"
#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/Json.hpp"


namespace
{
    static const int JSON_SPACE_INDENTATION = 2;
}

namespace ke
{
    EntitySptr JsonEntityBuilder::create(const std::any & p_parameters)
    {
        const auto & entityJsonObject = std::any_cast<const ke::json&>(p_parameters);

        // validate JSON object type
        auto jsonObjectTypeIt = entityJsonObject.find("type");
        if (jsonObjectTypeIt == entityJsonObject.end())
        {
            Log::instance()->error("Invalid JSON object received by EntityFactory::createNew. Content: {}", entityJsonObject.dump(JSON_SPACE_INDENTATION));
            return nullptr;
        }
        auto typeValue = (*jsonObjectTypeIt).get<std::string>();
        if (typeValue != "entity")
        {
            Log::instance()->error("Invalid JSON object type received by EntityFactory::createNew. Type given: '{}'", typeValue);
            return nullptr;
        }

        // set the entity name if it exists.
        ke::EntitySptr newEntity = ke::makeEntity(ke::Entity::newId());
        if (entityJsonObject.find("name") != entityJsonObject.end())
        {
            newEntity->setName(entityJsonObject["name"]);
        }

        // look for components array
        auto componentsArrayIt = entityJsonObject.find("components");
        if (componentsArrayIt == entityJsonObject.end())
        {
            Log::instance()->warn("Entity JSON object does not contain a components array.");
            return newEntity;
        }

        auto componentsArray = *componentsArrayIt;
        for (const auto & entityComponentJsonObj : componentsArray)
        {
            // validate entity component JSON object type
            jsonObjectTypeIt = entityComponentJsonObj.find("type");
            if (jsonObjectTypeIt == entityComponentJsonObj.end())
            {
                Log::instance()->warn("JSON object in entity component array for entity named '{}' does not have an object type. Component ignored.");
                continue;
            }
            typeValue = (*jsonObjectTypeIt).get<std::string>();
            if (typeValue != "entity_component")
            {
                Log::instance()->error("Unexpected JSON object type '{}' in entity component array for entity named '{}'.", typeValue, newEntity->getName());
                return nullptr;
            }
            auto componentTypeNameIt = entityComponentJsonObj.find("component_name");
            if (componentTypeNameIt == entityComponentJsonObj.end())
            {
                Log::instance()->error("Missing 'type_name' key value pair in entity component JSON object in entity component array for entity named '{}' does not have an object type.", newEntity->getName());
                return nullptr;
            }
            auto typeNameValue = (*componentTypeNameIt).get<std::string>();
            if (typeNameValue.length() == 0)
            {
                Log::instance()->error("Empty 'type_name' value in entity component JSON object in entity component array for entity named '{}' does not have an object type.", newEntity->getName());
                return nullptr;
            }

            auto componentLoader = this->entityFactory->getComponentBuilder(typeNameValue);
            auto entityComponent = componentLoader->build(entityComponentJsonObj);
            newEntity->addComponent(entityComponent);
        }

        if (!newEntity->initialise())
        {
            ke::Log::instance()->error("Entity initilisation failure.");
        }

        return newEntity;
    }
}