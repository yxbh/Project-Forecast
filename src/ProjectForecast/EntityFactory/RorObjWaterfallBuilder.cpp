#include "RorObjWaterfallBuilder.hpp"

#include "../AssetResources/GMSRoomResource.hpp"

#include "KEngine/App.hpp"

#include <cassert>


namespace pf
{
    ke::Entity * RorObjWaterfallBuilder::build(const std::any & p_parameters)
    {
        const auto & roomObjInfo = std::any_cast<const pf::GMSRoomObjectInstance&>(p_parameters);

        const auto resource = ke::App::instance()->getResourceManager()->getResource("oWaterfall");
        assert(resource);
        if (resource)
        {
            auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            auto entity = entityManager->newEntity(roomObjInfo.instanceid).lock().get();
            assert(entity);

            // TODO: build the water fall entity.

            return entity;
        }
        return nullptr;
    }
}