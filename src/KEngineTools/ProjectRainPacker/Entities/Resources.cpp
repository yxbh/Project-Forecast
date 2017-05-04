#include "Resources.hpp"

void to_json(ke::Json & jsonObject, const std::shared_ptr<KengineResource> & resource)
{
    jsonObject = ke::Json
    {
        { "name", resource->getName() },
        { "type", KengineJsonTypes::Resource },
        { "resource_type", ResourceTypes::Unknown },
        { "source_path", resource->getSourcePath() }
    };
}

void from_json(const ke::Json & jsonObject, std::shared_ptr<KengineResource> & resource)
{
    resource->name = jsonObject["name"].get<ke::String>();
    resource->sourcePath = jsonObject["source_path"].get<ke::String>();
}

void to_json(ke::Json & jsonObject, const std::shared_ptr<ImageResource> & resource)
{
    jsonObject = ke::Json
    {
        { "name", resource->getName() },
        { "type", KengineJsonTypes::Resource },
        { "resource_type", resource->getType() },
        { "source_path", resource->getSourcePath() }
    };
}

void from_json(const ke::Json & jsonObject, std::shared_ptr<ImageResource> & resource)
{
    resource->name = jsonObject["name"].get<ke::String>();
    resource->sourcePath = jsonObject["source_path"].get<ke::String>();
}
