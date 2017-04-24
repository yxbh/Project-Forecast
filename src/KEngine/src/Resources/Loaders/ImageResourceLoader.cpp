#include "KEngine/Resources/Loaders/ImageResourceLoader.hpp"

#include "KEngine/Resources/ImageResource.hpp"

namespace ke
{

    ResourceSptr ImageResourceLoader::load(const Json & resourceJsonObject)
    {
        return ke::ImageResource::create(resourceJsonObject);
    }

}