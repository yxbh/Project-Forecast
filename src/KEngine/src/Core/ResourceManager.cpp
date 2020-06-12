#include "KEngine/Core/ResourceManager.hpp"

#include "KEngine/Resources/ImageResource.hpp"
#include "KEngine/Resources/Loaders/ImageResourceLoader.hpp"

#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/Json.hpp"

#include <filesystem>
#include <fstream>

namespace ke
{
    static auto logger = ke::Log::createDefaultLogger("ResourceManager");

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

    void ResourceManager::releaseResource(ResourceSptr resource)
    {
        std::unique_lock lock(this->allResourcesMutex);
        this->allResources.erase(resource->getName());
    }

    void ResourceManager::releaseResource(const ke::String & resourceName)
    {
        std::unique_lock lock(this->allResourcesMutex);
        this->allResources.erase(resourceName);
    }

    void ResourceManager::registerResource(ResourceSptr resource)
    {
        assert(!resource->getName().empty());
        std::unique_lock lock(this->allResourcesMutex);
        if (this->allResources.count(resource->getName())) // TODO: replace with contains() from C++20.
        {
            // Check and log if we are replacing an existing resource.
            // We use warn level at the moment as replacing an existing resource could be an error.
            // Although in the future we may support hot reloading or custom resource override (i.e. mods) so
            // warn level may be too severe for that.
            auto oldResource = this->allResources[resource->getName()];
            if (oldResource->getSourcePath() == resource->getSourcePath())
            {
                logger->warn("Replacing resource with matching name \"{}\" from same source path: {}", resource->getSourcePath());
            }
            else
            {
                logger->warn("Resource with name \"{}\" already exists. It will be replaced.\nOld source path: {}\nNew source path: {}",
                    resource->getName(), oldResource->getSourcePath(), resource->getSourcePath());
            }
        }
        this->allResources.insert_or_assign(resource->getName(), resource);
    }

    ResourceSptr ResourceManager::getResource(const ke::String & name)
    {
        assert(!name.empty());
        std::shared_lock lock(this->allResourcesMutex);
        if (auto itr = this->allResources.find(name); itr != this->allResources.end())
        {
            return itr->second;
        }
        logger->error("Requesting non-existing resource: {}", name);
        return nullptr;
    }

    void ResourceManager::loadFromManifest(ke::String manifestPath)
    {
        // filesystem safety checks.
        namespace fs = std::filesystem;
        if (!fs::exists(manifestPath))
        {
            logger->error("Manifest path does not exist at: '{}'.", manifestPath);
            return;
        }
        if (!fs::is_regular_file(manifestPath))
        {
            logger->error("Manifest path is not a file. Path: '{}'.", manifestPath);
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
            logger->warn("Resource manifest at {} does not have a 'resources' object.");
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
                logger->error("No resource loader available for resource of type '{}'.", resourceType);
                continue;
            }

            auto resourceName = resourceJson["resource_name"].get<std::string>();
            auto newResource = resourceLoaderItr->second->load(resourceJson);
            if (nullptr == newResource)
            {
                logger->error("Resource loader failed to load resource named '{}'. JSON content: {}",
                    resourceName, resourceJson.dump(2));
                continue;
            }
            
            if (this->allResources.find(resourceName) != this->allResources.end())
            {
                logger->warn("Overwriting resource named '{}'.", resourceName);
            }

            std::unique_lock lock(this->allResourcesMutex);
            this->allResources[resourceName] = resourceLoaderItr->second->load(resourceJson);
        }
    }

    bool ResourceManager::validateManifestJson(const ke::Json & manifestJson, const ke::String & manifestPath)
    {
        auto manifestTypeItr = manifestJson.find("type");
        if (manifestTypeItr == manifestJson.end())
        {
            logger->error("Manifest JSON missing 'type' object.");
            return false;
        }
        if (manifestTypeItr->get<std::string>() != "manifest")
        {
            logger->error("The JSON at '{}' is not an engine resource manifest.", manifestPath);
            return false;
        }

        return true;
    }

    bool ResourceManager::validateResourceJson(const ke::Json & resourceJson)
    {
        auto typeItr = resourceJson.find("type");
        if (typeItr == resourceJson.end())
        {
            logger->error("Engine resource JSON missing 'type'. Content: {}", resourceJson.dump(2));
            return false;
        }
        auto typeValue = typeItr->get<std::string>();
        if (typeValue != "resource")
        {
            logger->error("Engine resource JSON containts unexpected 'type' value: {}", typeValue);
            return false;
        }

        auto resourceTypeItr = resourceJson.find("resource_type");
        if (resourceTypeItr == resourceJson.end())
        {
            logger->error("Engine resource JSON missing 'resource_type'. Content: {}", resourceJson.dump(2));
            return false;
        }
        auto resourceType = resourceTypeItr->get<std::string>();

        auto resourceNameItr = resourceJson.find("resource_name");
        if (resourceNameItr == resourceJson.end())
        {
            logger->error("Engine resource JSON missing 'resource_name'. Content: {}", resourceJson.dump(2));
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
            logger->info("Current resource loader for type '{}' resources is being replaced.", resourceType);
        }
        this->resourceLoaders[resourceType] = loader;
    }

}