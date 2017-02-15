#include "KEngine/Core/SystemManager.hpp"

#include "KEngine/Log/Log.hpp"

#include <cassert>

namespace ke
{
    SystemManager::~SystemManager()
    {
        this->clear();
    }

    bool SystemManager::addSystem(ke::SystemUptr && newSystem)
    {
        assert(newSystem);
        if (this->systems.find(newSystem->getType()) != this->systems.end())
        {
            ke::Log::instance()->error("System '{}' of type ({}) already exists in the system manager.",
                                        newSystem->getName(), newSystem->getType());
            return false;
        }
        auto systemRawPtr = newSystem.get();
        this->systems[newSystem->getType()] = std::move(newSystem);

        auto ret = systemRawPtr->initialise();
        if (!ret)
        {
            Log::instance()->error("{} failed to initialise.", systemRawPtr->getName());
        }

        return ret;
    }

    void SystemManager::removeSystem(ke::SystemUptr && newSystem)
    {
        assert(newSystem);
    }

    void SystemManager::clear()
    {
        for (auto & system : this->systems)
        {
            system.second->shutdown();
        }
        this->systems.clear();
    }

    void SystemManager::update(ke::Time elapsedTime)
    {
        for (auto & system : this->systems)
        {
            system.second->update(elapsedTime);
        }
    }

}