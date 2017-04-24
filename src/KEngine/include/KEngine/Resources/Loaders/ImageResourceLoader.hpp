#pragma once

#include "KEngine/Interfaces/IResourceLoader.hpp"

namespace ke
{
    
    class ImageResourceLoader : public ke::IResourceLoader
    {
    public:
        using IResourceLoader::IResourceLoader;

        virtual ~ImageResourceLoader() {}

        virtual ResourceSptr load(const Json & resourceJsonObject) override;

    };

    /// <summary>
    /// Helper template function for constructing a resource laoder object that's a subclass of ke::IResourceLoader.
    /// </summary>
    /// <param name="...args"></param>
    /// <returns></returns>
    template <typename ResourceLoader_T, typename ... Args_T>
    auto makeResourceLoader(Args_T && ... args) -> std::shared_ptr<ResourceLoader_T>
    {
        return std::make_shared<ResourceLoader_T>(std::forward<Args_T>(args)...);
    }

}