#pragma once

#include "IEntityComponent.hpp"
#include "KEngine/Common/Json.hpp"

#include <memory>

namespace ke
{

    /// <summary>
    /// Interface for implementing entity component creators that create components from some JSON object.
    /// </summary>
    class IEntityComponentJsonLoader
    {
    public:
        virtual ~IEntityComponentJsonLoader() = 0;

        virtual ke::EntityComponentSptr loadComponent(const ke::json & jsonObject) = 0;
    };

    inline IEntityComponentJsonLoader::~IEntityComponentJsonLoader() {}

    using EntityComponentJsonLoaderUptr = std::unique_ptr<IEntityComponentJsonLoader>;
    using EntityComponentJsonLoaderSptr = std::shared_ptr<IEntityComponentJsonLoader>;
    using EntityComponentJsonLoaderWptr = std::weak_ptr<IEntityComponentJsonLoader>;

}