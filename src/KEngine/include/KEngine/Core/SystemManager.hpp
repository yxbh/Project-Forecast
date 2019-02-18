#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Interfaces/ISystem.hpp"

#include <unordered_map>
#include <vector>

namespace ke
{

    /// <summary>
    /// A container that manages ke::ISystem objects.
    /// Note: this class is not thread-safe.
    /// </summary>
    class SystemManager
    {
    public:
        using SystemMap = std::unordered_map<ke::SystemType, ke::SystemUptr>;
        using SystemList = std::vector<ke::ISystem*>;

        ~SystemManager();

        /// <summary>
        /// Add the given system to the manager and initialise it.
        /// The new system is ignored if a system of the same type is already in the SystemManager.
        /// </summary>
        /// <param name="newSystem"></param>
        /// <returns>true if the system was aadded successfully or false if the initialisation failed or a system of the same type already exists.</returns>
        bool addSystem(ke::SystemUptr && newSystem);
        void removeSystem(ke::SystemUptr && newSystem);

        void clear();

        /// <summary>
        /// Call update on all current systems.
        /// </summary>
        /// <param name="elapsedTime"></param>
        void update(ke::Time elapsedTime);

        /// <summary>
        /// Get the number of systems being managed by the manager.
        /// </summary>
        inline std::size_t getSystemCount(void) const { return this->systems.size(); }

    private:
        SystemMap systemsMap;
        SystemList systems;
    };

    
}