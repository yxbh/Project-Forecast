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
        
        ke::Log::instance()->info("{} initialising ...", systemRawPtr->getName());
        auto ret = systemRawPtr->initialise();
        if (ret)
        {
            ke::Log::instance()->info("{} initialised.", systemRawPtr->getName());
        }
        else
        {
            Log::instance()->error("{} failed to initialise.", systemRawPtr->getName());
        }

        return ret;
    }

    void SystemManager::removeSystem(ke::SystemUptr && systemToRemove)
    {
        assert(systemToRemove);
        auto systemIt = this->systems.find(systemToRemove->getType());
        if (systemIt == this->systems.end())
        {
#if defined(KE_DEBUG)
            ke::Log::instance()->error("System of type ({}) does not exist in the system manager.", systemToRemove->getType());
#endif
            return;
        }

        auto system = systemIt->second.get();
        if (system == systemToRemove.get())
        {
#if defined(KE_DEBUG)
            ke::Log::instance()->error("Mismatching system of the same type ({}). Input system: {}. Found: {}", system->getType(), systemToRemove->getName(), system->getName());
#endif
            return;
        }

        ke::Log::instance()->info("Shutting down {} ...", system->getName());
        system->shutdown();
        this->systems.erase(systemIt);
    }

    void SystemManager::clear()
    {
        for (auto & system : this->systems)
        {
            ke::Log::instance()->info("Shutting down {} ...", system.second->getName());
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