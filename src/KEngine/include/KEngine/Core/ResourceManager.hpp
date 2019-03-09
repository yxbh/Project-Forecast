#pragma once

#include "KEngine/Interfaces/IResource.hpp"
#include "KEngine/Interfaces/IResourceLoader.hpp"

#include "KEngine/Common/ThreadPool.hpp"
#include "KEngine/Common/String.hpp"

#include <forward_list>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class ResourceManager
    {
    public:
        using ResourceId = ke::String;
        using ResourceLoaderMap = std::unordered_map<ke::ResourceType, ke::ResourceLoaderSptr>;
        using ResourceMap = std::unordered_map<ke::String, ke::ResourceSptr>;

        ResourceManager();
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager(ResourceManager &&) = default;
        ResourceManager & operator=(const ResourceManager &) = delete;
        ResourceManager & operator=(ResourceManager &&) = default;
        virtual ~ResourceManager();

        void update();

        bool isLoading() const;

        void loadFromManifest(ke::String manifestPath);

        /// <summary>
        /// Release ownership of the specified resource.
        /// </summary>
        void releaseResource(ResourceSptr resource);
        void releaseResource(const ke::String & resourceName);

        void registerResource(ResourceSptr resource);

        /// <summary>
        /// Get the resource with the given name.
        /// </summary>
        /// <param name="name">Resource name.</param>
        /// <returns>resource if a resource with the given name exists. nullptr is returned otherwise.</returns>
        ResourceSptr getResource(const ke::String & name);

        void registerResourceLoader(ke::ResourceType resourceType, ke::ResourceLoaderSptr loader);

    private:
        bool validateManifestJson(const ke::Json & manifestJson, const ke::String & manifestPath);
        bool validateResourceJson(const ke::Json & resourceJson);

        using LoadResultFuture = std::future<bool>;

        ThreadPool threadPool;
        std::forward_list<LoadResultFuture> threadedInProgressLoadingFutures;

        ResourceLoaderMap resourceLoaders;
        ResourceMap allResources;
        std::shared_mutex allResourcesMutex;

    };

    inline ResourceManager::~ResourceManager() {}


    using ResourceManagerUptr = std::unique_ptr<ResourceManager>;
    using ResourceManagerSptr = std::shared_ptr<ResourceManager>;
    using ResourceManagerWptr = std::weak_ptr<ResourceManager>;


}