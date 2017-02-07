#pragma once

namespace ke
{
    
    /// <summary>
    /// Interface for implementing systems.
    /// </summary>
    class ISystem
    {
    public:
        virtual ~ISystem() = 0;
    };

    inline ISystem::~ISystem() {}

}