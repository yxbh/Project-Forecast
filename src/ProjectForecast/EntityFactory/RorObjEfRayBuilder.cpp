#include "RorObjEfRayBuilder.hpp"

#include "../AssetResources/GMSRoomResource.hpp"
#include "../AssetResources/GMSObjectResource.hpp"
#include "../AssetResources/OtherGMSResources.hpp"
#include "../Utility/GraphicsHelper.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Graphics/SceneNodes.hpp"
#include "KEngine/Utility/StringHelper.hpp"
#include "KEngine/Log/Log.hpp"

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>


namespace pf
{
    ke::Entity * RorObjEfRayBuilder::build(const std::any & p_parameters)
    {
        static constexpr auto OBJECT_RESOURCE_NAME = "oEfRay";

        const auto & roomObjInfo = std::any_cast<const pf::GMSRoomObjectInstance&>(p_parameters);

        auto resourceManager = ke::App::instance()->getResourceManager();
        const auto objectResource = std::static_pointer_cast<pf::GMSObjectResource>(resourceManager->getResource(OBJECT_RESOURCE_NAME));
        assert(objectResource);
        if (objectResource)
        {
            //auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            //auto entity = entityManager->newEntity(roomObjInfo.instanceid).lock();
            //assert(entity);
            //entity->setName(OBJECT_RESOURCE_NAME);


            //auto renderableComponent = ke::makeEntityComponent<ke::EntityRenderableComponent>(entity);
            //renderableComponent->setSceneNode(geyserNode);
            //entity->addComponent(renderableComponent);
            //
            //return entity.get();
            return nullptr;
        }
        else
        {
            ke::Log::instance()->error("Cannot instantiate entity. Missing resource: {}", OBJECT_RESOURCE_NAME);
        }
        return nullptr;
    }
}