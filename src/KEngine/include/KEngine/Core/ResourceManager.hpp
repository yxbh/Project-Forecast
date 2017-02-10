#pragma once

#include "KEngine/Common/ThreadPool.hpp"
#include "KEngine/Common/String.hpp"

#include <forward_list>
#include <memory>

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class ResourceManager
    {
    public:
        using ResourceId = ke::String;

        ResourceManager();
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager(ResourceManager &&) = default;
        ResourceManager & operator=(const ResourceManager &) = delete;
        ResourceManager & operator=(ResourceManager &&) = default;
        virtual ~ResourceManager();

        /// <summary>
        /// 
        /// </summary>
        void update();

        bool isLoading() const;

        // TODO: getResource() and pre/loadResource() methods.

    private:
        using LoadResultFuture = std::future<bool>;

        ThreadPool threadPool;
        std::forward_list<LoadResultFuture> threadedInProgressLoadingFutures;
    };

    inline ResourceManager::~ResourceManager() {}


    using ResourceManagerUptr = std::unique_ptr<ResourceManager>;
    using ResourceManagerSptr = std::shared_ptr<ResourceManager>;
    using ResourceManagerWptr = std::weak_ptr<ResourceManager>;


}