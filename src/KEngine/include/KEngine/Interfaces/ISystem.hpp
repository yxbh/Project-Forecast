#pragma once

#include "KEngine/Common/Time.hpp"

#include <memory>


namespace ke
{

    using SystemType = std::uint32_t;


    /// <summary>
    /// Interface for implementing systems.
    /// </summary>
    class ISystem
    {
    public:
        static constexpr const SystemType INVALID_SYSTEM_TYPE = 0;

        virtual ~ISystem() = 0;

        virtual bool initialise() = 0;
        virtual void shutdown() = 0;

        virtual void update(ke::Time elapsedTime) = 0;

        virtual ke::SystemType getType() const = 0;
        virtual const char * getName() const = 0;
    };

    inline ISystem::~ISystem() {}

    using SystemUptr = std::unique_ptr<ISystem>;
    using SystemSptr = std::shared_ptr<ISystem>;
    using SystemWptr = std::weak_ptr<ISystem>;

}

/// <summary>
/// Define the common ISystem properties.
/// </summary>
#define KE_DEFINE_SYSTEM_COMMON_PROPERTIES(CLASS_NAME, SYSTEM_TYPE_VALUE) \
public: \
    static constexpr ke::SystemType TYPE = SYSTEM_TYPE_VALUE; \
    static constexpr const char * const NAME = #CLASS_NAME; \
    virtual ke::SystemType getType() const override { return CLASS_NAME::TYPE; } \
    virtual const char * getName() const override { return CLASS_NAME::NAME; } \
private:
