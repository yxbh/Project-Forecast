#include "EntityFactorySystem.hpp"

#include "../EntityFactory/RorObjGeyserBuilder.hpp"
#include "../EntityFactory/RorObjWaterfallBuilder.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Common/macros.hpp"

namespace pf
{
    bool EntityFactorySystem::initialise(void)
    {
        auto factory = ke::App::instance()->getLogic()->getEntityFactory();
        factory->registerEntityBuilder("oGeyser", std::make_unique<RorObjGeyserBuilder>(factory));
        factory->registerEntityBuilder("oWaterfall", std::make_unique<RorObjWaterfallBuilder>(factory));
        
        return true;
    }

    void EntityFactorySystem::shutdown(void)
    {
        // nothing to do here.
    }

    void EntityFactorySystem::update(ke::Time p_elapsedTime)
    {
        // nothing to do here.
        KE_UNUSED(p_elapsedTime);
    }
}