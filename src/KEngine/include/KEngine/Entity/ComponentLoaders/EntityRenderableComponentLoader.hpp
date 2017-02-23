#pragma once

#include "KEngine/Interface/IEntityComponentJsonLoader.hpp"

namespace ke
{

    class EntityRenderableComponentLoader : public ke::IEntityComponentJsonLoader
    {
    public:
        virtual ke::EntityComponentSptr loadComponent(const ke::json & jsonObject) override;
    };

}