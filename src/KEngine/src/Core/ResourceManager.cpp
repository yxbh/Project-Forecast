#include "KEngine/Core/ResourceManager.hpp"

namespace ke
{

    ResourceManager::ResourceManager()
        : threadPool(2)
    {

    }

    void ResourceManager::update()
    {
        // TODO
    }

    bool ResourceManager::isLoading() const
    {
        return !threadedInProgressLoadingFutures.empty();
    }

}