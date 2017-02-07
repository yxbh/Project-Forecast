#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/Entity.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{

}

TEST_CASE("ke::Entity Unit Tests")
{
    ke::Entity entity;
    CHECK(entity.getId() == ke::INVALID_ENTITY_ID);

    CHECK(entity.getName() == KE_TEXT(""));
    CHECK(entity.getTags().size() == 0);

    entity.setName(KE_TEXT("TestEntity"));
    CHECK(entity.getName() == KE_TEXT("TestEntity"));

    CHECK(!entity.hasTag(KE_TEXT("TestTag")));
    entity.addTag(KE_TEXT("TestTag"));
    CHECK(entity.hasTag(KE_TEXT("TestTag")));
    entity.addTag(KE_TEXT("TestTag2"));
    CHECK(entity.hasTag(KE_TEXT("TestTag")));
    CHECK(entity.hasTag(KE_TEXT("TestTag2")));
    CHECK(entity.getTags().size() == 2);

    CHECK(ke::Entity::newId() != ke::INVALID_ENTITY_ID);

    ke::Entity entity2(ke::Entity::newId());
    CHECK(entity2.getId() != ke::INVALID_ENTITY_ID);
}

TEST_CASE("ke::IEntityComponent Unit Tests")
{

    SECTION("")
    {
    }
}

#endif
