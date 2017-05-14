#include "KEngine/Resources/ImageResource.hpp"

#include <SFML/Graphics/Image.hpp>

namespace ke
{

    std::shared_ptr<ImageResource> ImageResource::create(const ke::Json & jsonObject)
    {
        auto newResource = makeResource<ImageResource>();
        newResource = jsonObject;
        return newResource;
    }

    void to_json(ke::Json & jsonObject, const std::shared_ptr<ImageResource> & resource)
    {
        jsonObject = ke::Json
        {
            { "resource_name", resource->getName() },
            { "source_path", resource->getSourcePath() }
        };
    }

    void from_json(const ke::Json & jsonObject, std::shared_ptr<ImageResource> & resource)
    {
        resource->name = jsonObject["resource_name"].get<ke::String>();
        resource->sourcePath = jsonObject["source_path"].get<ke::String>();
    }

}