#pragma once

#include "IResource.hpp"

#include "KEngine/Common/Json.hpp"

#include <memory>

namespace ke
{

    /// <summary>
    /// Interface class for implementing custom resource loaders used by
    /// KEngine's ke::ResourceManager.
    /// </summary>
    class IResourceLoader
    {
    public:
        virtual ~IResourceLoader() = 0;

        virtual ResourceSptr load(const Json & resourceJsonObject) = 0;
    };

    inline IResourceLoader::~IResourceLoader() {}

    using ResourceLoaderUptr = std::unique_ptr<IResourceLoader>;
    using ResourceLoaderSptr = std::shared_ptr<IResourceLoader>;
    using ResourceLoaderWptr = std::weak_ptr<IResourceLoader>;

}