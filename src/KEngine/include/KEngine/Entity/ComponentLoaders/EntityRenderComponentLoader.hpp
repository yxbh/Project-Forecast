#pragma once

#include "KEngine/Interface/IEntityComponentJsonLoader.hpp"

namespace ke
{

    class EntityRenderComponentLoader : public ke::IEntityComponentJsonLoader
    {
    public:
        virtual ke::EntityComponentSptr loadComponent(const ke::json & jsonObject) override;
    };

}