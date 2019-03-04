#pragma once

#include "KEngine/Interfaces/IEntityBuilder.hpp"

namespace pf
{
    /// <summary>
    /// An entity builder for instantiating 
    /// </summary>
    class RorObjWaterfallBuilder : public ke::IEntityBuilder
    {
    public:
        using IEntityBuilder::IEntityBuilder;

        ke::Entity * build(const std::any & p_parameters) final;
    };
}