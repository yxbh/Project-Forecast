#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EntityManager.hpp"

#include "KEngine/UnitTest/catch.hpp"


TEST_CASE("ke::EntityManager Unit Tests")
{

    SECTION("Add & Search")
    {
        ke::EntityManager em;

        auto entity = ke::makeEntity(ke::Entity::newId());

        CHECK(em.findEntityById(entity->getId()).expired());

        em.addEntity(entity);

        CHECK(!em.findEntityById(entity->getId()).expired());

        auto entitySearchRetSptr = em.findEntityById(entity->getId()).lock();
        CHECK(entitySearchRetSptr != nullptr);
        CHECK(entity->getId() == entitySearchRetSptr->getId());
    }

}

#endif