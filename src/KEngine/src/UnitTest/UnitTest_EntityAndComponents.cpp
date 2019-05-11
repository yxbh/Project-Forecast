#if defined(RUN_UNIT_TESTS)

#include "KEngine/Interfaces/IEntityComponent.hpp"
#include "KEngine/Core/EntityManager.hpp"
#include "KEngine/Core/Entity.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{

    class TestComponent : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(TestComponent, 0xAE3DEA1D)

    public:
    };

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

	ke::EntityManager entityManager;
    auto entity2 = entityManager.newEntity(entityManager.newId()).lock();
	CHECK(entity2 != nullptr);
    CHECK(entity2->getId() != ke::INVALID_ENTITY_ID);
}

TEST_CASE("ke::IEntityComponent Unit Tests")
{

    SECTION("")
    {
    }
}

#endif
