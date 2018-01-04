#include "DebugSceneGridSystem.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Log/Log.hpp"

namespace pf
{

    bool DebugSceneGridSystem::initialise()
    {
        //ke::EventManager::registerListener<pf::RequestDrawDebugDotEvent>(this, &DebugSceneGridSystem::handleRequest);

        auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
        auto xlineEntity = entityManager->newEntity().lock();
        auto ylineEntity = entityManager->newEntity().lock();
        static const ke::Point2DFloat xlineBegin{ 0, 5000.f };
        static const ke::Point2DFloat xlineEnd{ 0, -5000.f };
        static const ke::Point2DFloat ylineBegin{ 5000.f, 0 };
        static const ke::Point2DFloat ylineEnd{ -5000.f, 0 };
        static const int16_t depth = 50;
        static const auto lineColour = ke::Color::BLUE;
        xlineEntity->addComponent(ke::makeEntityComponent<ke::LineDrawableComponent>(xlineEntity, xlineBegin, xlineEnd, depth, lineColour));
        ylineEntity->addComponent(ke::makeEntityComponent<ke::LineDrawableComponent>(ylineEntity, ylineBegin, ylineEnd, depth, lineColour));

        this->gridLineEntities.push_back(xlineEntity);
        this->gridLineEntities.push_back(ylineEntity);

        return true;
    }

    void DebugSceneGridSystem::shutdown()
    {
        //ke::EventManager::deregisterListener<pf::RequestDrawDebugDotEvent>(this, &DebugSceneGridSystem::handleRequest);

        auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
        for (auto entity : this->gridLineEntities)
        {
            auto e = entity.lock();
            if (e)
            {
                entityManager->removeEntity(e->getId());
            }
        }
        this->gridLineEntities.clear();
    }

    void DebugSceneGridSystem::update(ke::Time elapsedTime)
    {

    }

    void DebugSceneGridSystem::handleRequest(ke::EventSptr event)
    {
    }

}
