#pragma once

#include "KEngine/Common/String.hpp"

#include <cstdint>
#include <memory>

namespace ke
{

    using ResourceTypeInternalType = char * const;
    using ResourceType = std::string;

    /// <summary>
    /// An interface class for implementing resources used by KEngine.
    /// Use the KE_DEFINE_RESOURCE_COMMON_PROPERTIES when defining a new resource type.
    /// Related classes: ke::ResourceManager
    /// </summary>
    class IResource
    {
    public:
        using Sptr = std::shared_ptr<IResource>;
        using Uptr = std::unique_ptr<IResource>;
        using Wptr = std::weak_ptr<IResource>;

        static constexpr ResourceTypeInternalType INVALID_RESOURCE = "invalid_type";
        static constexpr ResourceTypeInternalType TYPE = INVALID_RESOURCE; /// each class inheriting from IResource is required to redefine this.

        IResource();
        IResource(IResource &) = delete;
        IResource(IResource &&) = delete;
        IResource & operator=(IResource &) = delete;
        IResource & operator=(IResource &&) = delete;

        virtual ~IResource();

        virtual ke::ResourceType getType() const = 0;
        inline ke::String getName() const { return this->name; }
        inline ke::String getSourcePath() const { return this->sourcePath; }

        inline void setName(const ke::String & newName) { this->name = newName; }
        inline void setSourcePath(const ke::String & newSourcePath) { this->sourcePath = newSourcePath; }

    protected:
        ke::String name;
        ke::String sourcePath;
    };

    inline IResource::IResource() {}
    inline IResource::~IResource() {}
    inline ResourceType IResource::getType() const
    {
        return INVALID_RESOURCE;
    }


    using ResourceSptr = IResource::Sptr;
    using ResourceUptr = IResource::Uptr;
    using ResourceWptr = IResource::Wptr;


    /// <summary>
    /// Helper template function for constructing a resource object that's a subclass of ke::IResource.
    /// </summary>
    /// <param name="...args"></param>
    /// <returns></returns>
    template <typename Resource_T, typename ... Args_T>
    auto makeResource(Args_T && ... args) -> std::shared_ptr<Resource_T>
    {
        return std::make_shared<Resource_T>(std::forward<Args_T>(args)...);
    }

}

/// <summary>
/// Define the common IResource properties.
/// </summary>
#define KE_DEFINE_RESOURCE_COMMON_PROPERTIES(CLASS_NAME, RESOURCE_TYPE_VALUE) \
public: \
    static constexpr ke::ResourceTypeInternalType TYPE = RESOURCE_TYPE_VALUE; \
    virtual ke::ResourceType getType() const override { return CLASS_NAME::TYPE; } \
private: