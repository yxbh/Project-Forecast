#pragma once

#include "KEngine/Interfaces/IEntityBuilder.hpp"

namespace pf
{
    /// <summary>
    /// An entity builder for instantiating a geyser entity.
    /// </summary>
    class RorObjEfRayBuilder : public ke::IEntityBuilder
    {
    public:
        using IEntityBuilder::IEntityBuilder;

        ke::Entity * build(const std::any & p_parameters) final;
    };
}