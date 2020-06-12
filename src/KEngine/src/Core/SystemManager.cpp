#include "KEngine/Core/SystemManager.hpp"

#include "KEngine/Log/Log.hpp"

#include <cassert>

namespace ke
{
    static auto logger = ke::Log::createDefaultLogger("SystemManager");

    SystemManager::~SystemManager()
    {
        this->clear();
    }

    bool SystemManager::addSystem(ke::SystemUptr && newSystem)
    {
        assert(newSystem);
        if (this->systemsMap.find(newSystem->getType()) != this->systemsMap.end())
        {
            logger->error("System '{}' of type ({}) already exists in the system manager.",
                                        newSystem->getName(), newSystem->getType());
            return false;
        }
        auto systemRawPtr = newSystem.get();
        this->systemsMap[newSystem->getType()] = std::move(newSystem);
        this->systems.push_back(systemRawPtr);
        
        logger->info("{} initialising ...", systemRawPtr->getName());
        auto ret = systemRawPtr->initialise();
        if (ret)
        {
            logger->info("{} initialised ... DONE", systemRawPtr->getName());
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
        auto systemIt = this->systemsMap.find(systemToRemove->getType());
        if (systemIt == this->systemsMap.end())
        {
#if defined(KE_DEBUG)
            logger->error("System of type ({}) does not exist in the system manager.", systemToRemove->getType());
#endif
            return;
        }

        auto system = systemIt->second.get();
        if (system == systemToRemove.get())
        {
#if defined(KE_DEBUG)
            logger->error("Mismatching system of the same type ({}). Input system: {}. Found: {}", system->getType(), systemToRemove->getName(), system->getName());
#endif
            return;
        }

        logger->info("Shutting down {} ...", system->getName());
        system->shutdown();
        auto systemsNewEnd = std::remove(this->systems.begin(), this->systems.end(), system);
        assert(systemsNewEnd != this->systems.end());
        this->systems.erase(systemsNewEnd, this->systems.end());
        this->systemsMap.erase(systemIt);
    }

    void SystemManager::clear()
    {
        for (auto it = this->systems.rbegin(); it != this->systems.rend(); ++it)
        {
            auto & system = *it;
            logger->info("Shutting down {} ...", system->getName());
            system->shutdown();
        }
        this->systems.clear();
        this->systemsMap.clear();
    }

    void SystemManager::update(ke::Time elapsedTime)
    {
        for (auto system : this->systems)
        {
            system->update(elapsedTime);
        }
    }

}