#pragma once

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Common/String.hpp"

#include <any>
#include <cassert>
#include <memory>


namespace ke
{
    class EntityFactory;

    class IEntityBuilder
    {
    public:
        IEntityBuilder(EntityFactory * p_entityFactory) : entityFactory(p_entityFactory) { assert(p_entityFactory); }
        virtual ~IEntityBuilder(void) = 0;

        virtual ke::Entity * build(const std::any & p_parameters) = 0;

    protected:
        EntityFactory * entityFactory;
    };

    inline IEntityBuilder::~IEntityBuilder(void) {}

    using EntityBuilderUptr = std::unique_ptr<IEntityBuilder>;
    using EntityBuilderSptr = std::shared_ptr<IEntityBuilder>;
    using EntityBuilderWptr = std::weak_ptr<IEntityBuilder>;
}