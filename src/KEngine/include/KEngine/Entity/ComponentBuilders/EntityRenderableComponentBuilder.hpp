#pragma once

#include "KEngine/Interfaces/IEntityComponentBuilder.hpp"

namespace ke
{

    class EntityRenderableComponentBuilder : public ke::IEntityComponentBuilder
    {
    public:
        virtual ke::EntityComponentSptr build(const std::any & p_jsonObject) override;
    };

}