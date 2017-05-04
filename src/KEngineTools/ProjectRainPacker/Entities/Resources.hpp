#pragma once

#include "KEngine/Interfaces/IResource.hpp"
#include "KEngine/Common/Json.hpp"

#include "Logic/Constants.hpp"


class KengineResource : public ke::IResource
{
public:
    virtual ~KengineResource() {}

    void setType(const ke::String & newType) { this->resourceType = newType; }
    virtual ke::ResourceType getType() const override { return this->resourceType; }

    friend void to_json(ke::Json & jsonObject, const std::shared_ptr<KengineResource> & resource);
    friend void from_json(const ke::Json & json, std::shared_ptr<KengineResource> & resource);

protected:
    ke::String resourceType;
};

class ImageResource : public ke::IResource
{
    KE_DEFINE_RESOURCE_COMMON_PROPERTIES(ImageResource, ResourceTypes::Image)

public:
    virtual ~ImageResource() {}

    friend void to_json(ke::Json & jsonObject, const std::shared_ptr<ImageResource> & resource);
    friend void from_json(const ke::Json & json, std::shared_ptr<ImageResource> & resource);
};
