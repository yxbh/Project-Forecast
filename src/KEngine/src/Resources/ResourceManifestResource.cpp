#include "KEngine/Resources/ResourceManifestResource.hpp"

#include "KEngine/Log/Log.hpp"

namespace ke
{

    std::shared_ptr<ResourceManifestResource> ResourceManifestResource::create(const ke::Json & jsonObject)
    {
        auto newResource = makeResource<ResourceManifestResource>();
        newResource = jsonObject;
        return newResource;
    }

    ResourceManifestResource::ResourceList ResourceManifestResource::getAllResources() const
    {
        ResourceList resources;

        auto resourcesItr = this->manifestJson.find("resources");
        if (resourcesItr == this->manifestJson.end())
        {
            return resources;
        }

        for (const auto & resource : *resourcesItr)
        {
            auto typeItr = resource.find("type");
            if (typeItr == resource.end())
            {
                ke::Log::instance()->error("Engine resource JSON missing 'type'. Content: {}", resource.dump(2));
                continue;
            }

            auto resourceTypeItr = resource.find("resource_type");
            if (resourceTypeItr == resource.end())
            {
                ke::Log::instance()->error("Engine resource JSON missing 'resource_type'. Content: {}", resource.dump(2));
                continue;
            }


        }

        return resources;
    }

    void to_json(ke::Json & jsonObject, const std::shared_ptr<ResourceManifestResource> & resource)
    {
        jsonObject = ke::Json
        {
            { "name", resource->getName() },
            { "source_path", resource->getSourcePath() }
        };
    }

    void from_json(const ke::Json & jsonObject, std::shared_ptr<ResourceManifestResource> & resource)
    {
        resource->name = jsonObject["name"].get<ke::String>();
        resource->sourcePath = jsonObject["source_path"].get<ke::String>();
    }

}