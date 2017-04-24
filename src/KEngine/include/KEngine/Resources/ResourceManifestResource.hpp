#pragma once

#include "KEngine/Interfaces/IResource.hpp"
#include "KEngine/Common/Json.hpp"

#include <vector>

namespace ke
{
    
    class ResourceManifestResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(ResourceManifestResource, "manifest")

    public:
        using ResourceList = std::vector<ResourceSptr>;

        using IResource::IResource;

        std::shared_ptr<ResourceManifestResource> create(const ke::Json & jsonObject);

        inline const Json & getManifestAsJson() const { return this->manifestJson; }
        inline Json & getManifestAsJson() { return this->manifestJson; }

        ResourceList getAllResources() const;

        friend void to_json(ke::Json & jsonObject, const std::shared_ptr<ResourceManifestResource> & resource);
        friend void from_json(const ke::Json & json, std::shared_ptr<ResourceManifestResource> & resource);

    private:
        Json manifestJson;
    };

}