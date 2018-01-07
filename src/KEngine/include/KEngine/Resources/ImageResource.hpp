#pragma once

#include "KEngine/Interfaces/IResource.hpp"
#include "KEngine/Common/Json.hpp"

#include <memory>

namespace sf
{
    class Image;
}

namespace ke
{

    class ImageResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(ImageResource, "image")

    public:
        static std::shared_ptr<ImageResource> create(const ke::Json & jsonObject);

        using IResource::IResource;

        inline void * getImageData() { return this->imageData.get(); }

        friend void to_json(ke::Json & jsonObject, const std::shared_ptr<ImageResource> & resource);
        friend void from_json(const ke::Json & json, std::shared_ptr<ImageResource> & resource);

    private:
        std::unique_ptr<sf::Image> imageData;
    };

}