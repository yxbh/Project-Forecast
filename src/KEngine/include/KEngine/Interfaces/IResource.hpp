#pragma once

#include <cstdint>
#include <memory>

namespace ke
{

    using ResourceType = std::uint16_t;


    class IResource
    {
    public:
        using Sptr = std::shared_ptr<IResource>;
        using Uptr = std::unique_ptr<IResource>;
        using Wptr = std::weak_ptr<IResource>;

        static const ResourceType INVALID_RESOURCE = 0;
        static const ResourceType TYPE = INVALID_RESOURCE; /// each class inheriting from IResource is required to redefine this.

        IResource();
        IResource(IResource &) = delete;
        IResource(IResource &&) = delete;
        IResource & operator=(IResource &) = delete;
        IResource & operator=(IResource &&) = delete;

        virtual ~IResource();

        virtual ke::ResourceType getType() const = 0;
        virtual const char * getName() const = 0;
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
    static constexpr ke::ResourceType TYPE = RESOURCE_TYPE_VALUE; \
    static constexpr const char * const NAME = #CLASS_NAME; \
    virtual ke::ResourceType getType() const override { return CLASS_NAME::TYPE; } \
    virtual const char * getName() const override { return CLASS_NAME::NAME; } \
private: