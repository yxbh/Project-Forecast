#pragma once

#include "IEntityComponent.hpp"

#include <any>
#include <memory>

namespace ke
{

    /// <summary>
    /// Interface for implementing entity component creators that create components from some JSON object.
    /// </summary>
    class IEntityComponentBuilder
    {
    public:
        virtual ~IEntityComponentBuilder() = 0;

        virtual ke::EntityComponentSptr build(const std::any & p_parameters) = 0;
    };

    inline IEntityComponentBuilder::~IEntityComponentBuilder() {}

    using EntityComponentLoaderUptr = std::unique_ptr<IEntityComponentBuilder>;
    using EntityComponentLoaderSptr = std::shared_ptr<IEntityComponentBuilder>;
    using EntityComponentLoaderWptr = std::weak_ptr<IEntityComponentBuilder>;

}