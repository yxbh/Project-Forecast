#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EntityManager.hpp"

#include "KEngine/UnitTest/catch.hpp"


TEST_CASE("ke::EntityManager Unit Tests")
{

    SECTION("Add & Search")
    {
        ke::EntityManager em;

        auto entity = ke::makeEntity();

        REQUIRE(em.findEntityById(entity->getId()).expired());

        em.addEntity(entity);

        REQUIRE(!em.findEntityById(entity->getId()).expired());

        auto entitySearchRetSptr = em.findEntityById(entity->getId()).lock();
        REQUIRE(entitySearchRetSptr != nullptr);
        REQUIRE(entity->getId() == entitySearchRetSptr->getId());
    }

}

#endif