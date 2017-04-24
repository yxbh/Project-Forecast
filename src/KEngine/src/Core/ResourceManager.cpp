#include "KEngine/Core/ResourceManager.hpp"

#include "KEngine/Resources/ImageResource.hpp"
#include "KEngine/Resources/Loaders/ImageResourceLoader.hpp"

#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/Json.hpp"

#include <filesystem>
#include <fstream>

namespace ke
{

    ResourceManager::ResourceManager()
        : threadPool(2)
    {
        this->registerResourceLoader(ke::ImageResource::TYPE, ke::makeResourceLoader<ke::ImageResourceLoader>());
    }

    void ResourceManager::update()
    {
        // TODO
    }

    bool ResourceManager::isLoading() const
    {
        return !threadedInProgressLoadingFutures.empty();
    }

    void ResourceManager::release(ResourceSptr resource)
    {
        this->allResources.erase(resource->getName());
    }

    void ResourceManager::loadFromManifest(ke::String manifestPath)
    {
        // filesystem safety checks.
        namespace fs = std::experimental::filesystem;
        if (!fs::exists(manifestPath))
        {
            ke::Log::instance()->error("Manifest path does not exist at: '{}'.", manifestPath);
            return;
        }
        if (!fs::is_regular_file(manifestPath))
        {
            ke::Log::instance()->error("Manifest path is not a file. Path: '{}'.", manifestPath);
            return;
        }

        // read the JSON content.
        std::ifstream ifs{ manifestPath.c_str() };
        std::string content( std::istreambuf_iterator<char> { ifs }, std::istreambuf_iterator<char>{} );
        auto manifestJson = ke::Json::parse(content.begin(), content.end());

        // type check JSON for manifest
        if (!this->validateManifestJson(manifestJson, manifestPath))
        {
            return;
        }

        //  check if there are resources to load.
        auto resourcesItr = manifestJson.find("resources");
        if (resourcesItr == manifestJson.end())
        {
            ke::Log::instance()->warn("Resource manifest at {} does not have a 'resources' object.");
            return;
        }

        // load all resources.
        for (const auto & resourceJson : *resourcesItr)
        {
            if (!this->validateResourceJson(resourceJson))
            {
                continue;
            }

            auto resourceType = resourceJson["resource_type"].get<std::string>();
            auto resourceLoaderItr = this->resourceLoaders.find(resourceType);
            if (resourceLoaderItr == this->resourceLoaders.end())
            {
                ke::Log::instance()->error("No resource loader available for resource of type '{}'.", resourceType);
                continue;
            }

            auto resourceName = resourceJson["resource_name"].get<std::string>();
            auto newResource = resourceLoaderItr->second->load(resourceJson);
            if (nullptr == newResource)
            {
                ke::Log::instance()->error("Resource loader failed to load resource named '{}'. JSON content: {}",
                    resourceName, resourceJson.dump(2));
                continue;
            }
            
            if (this->allResources.find(resourceName) != this->allResources.end())
            {
                ke::Log::instance()->warn("Overwriting resource named '{}'.", resourceName);
            }

            this->allResources[resourceName] = resourceLoaderItr->second->load(resourceJson);
        }
    }

    bool ResourceManager::validateManifestJson(const ke::Json & manifestJson, const ke::String & manifestPath)
    {
        auto manifestTypeItr = manifestJson.find("type");
        if (manifestTypeItr == manifestJson.end())
        {
            ke::Log::instance()->error("Manifest JSON missing 'type' object.");
            return false;
        }
        if (manifestTypeItr->get<std::string>() != "manifest")
        {
            ke::Log::instance()->error("The JSON at '{}' is not an engine resource manifest.", manifestPath);
            return false;
        }

        return true;
    }

    bool ResourceManager::validateResourceJson(const ke::Json & resourceJson)
    {
        auto typeItr = resourceJson.find("type");
        if (typeItr == resourceJson.end())
        {
            ke::Log::instance()->error("Engine resource JSON missing 'type'. Content: {}", resourceJson.dump(2));
            return false;
        }

        auto resourceTypeItr = resourceJson.find("resource_type");
        if (resourceTypeItr == resourceJson.end())
        {
            ke::Log::instance()->error("Engine resource JSON missing 'resource_type'. Content: {}", resourceJson.dump(2));
            return false;
        }
        auto resourceType = resourceTypeItr->get<std::string>();

        auto resourceNameItr = resourceJson.find("resource_name");
        if (resourceNameItr == resourceJson.end())
        {
            ke::Log::instance()->error("Engine resource JSON missing 'resource_name'. Content: {}", resourceJson.dump(2));
            return false;
        }
        auto resourceName = resourceNameItr->get<std::string>();

        

        return true;
    }

    void ResourceManager::registerResourceLoader(ke::ResourceType resourceType, ke::ResourceLoaderSptr loader)
    {
        auto resourceLoaderItr = this->resourceLoaders.find(resourceType);
        if (resourceLoaderItr != this->resourceLoaders.end() &&
            resourceLoaderItr->second != loader)
        {
            ke::Log::instance()->info("Current resource loader for type '{}' resources is being replaced.", resourceType);
        }
        this->resourceLoaders[resourceType] = loader;
    }

}