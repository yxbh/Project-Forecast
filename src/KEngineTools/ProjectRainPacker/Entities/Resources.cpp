#include "Resources.hpp"

void to_json(ke::Json & jsonObject, const std::shared_ptr<KengineResource> & resource)
{
    assert(resource);
    jsonObject = ke::Json
    {
        { "resource_name", resource->getName() },
        { "type", KengineJsonTypes::Resource },
        { "resource_type", ResourceTypes::Unknown },
        { "source_path", resource->getSourcePath() }
    };
}

void from_json(const ke::Json & jsonObject, std::shared_ptr<KengineResource> & resource)
{
    resource = ke::makeResource<KengineResource>();
    resource->name = jsonObject["resource_name"].get<ke::String>();
    resource->sourcePath = jsonObject["source_path"].get<ke::String>();
}

void to_json(ke::Json & jsonObject, const std::shared_ptr<ImageResource> & resource)
{
    assert(resource);
    jsonObject = ke::Json
    {
        { "resource_name", resource->getName() },
        { "type", KengineJsonTypes::Resource },
        { "resource_type", resource->getType() },
        { "source_path", resource->getSourcePath() }
    };
}

void from_json(const ke::Json & jsonObject, std::shared_ptr<ImageResource> & resource)
{
    resource = ke::makeResource<ImageResource>();
    resource->name = jsonObject["resource_name"].get<ke::String>();
    resource->sourcePath = jsonObject["source_path"].get<ke::String>();
}
