#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Log/Log.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{

    class TestComponent1 : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(TestComponent1, 0x5C099729)

    public:
        using IEntityComponent::IEntityComponent;

        virtual bool initialise() override
        {
            return true;
        }

        virtual void postInitialise() override {}

        virtual void update(const ke::Time) override {}
    };

    class TestComponent2 : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(TestComponent2, 0xA52F3FBE)

    public:
        using IEntityComponent::IEntityComponent;

        virtual bool initialise() override
        {
            return true;
        }

        virtual void postInitialise() override {}

        virtual void update(const ke::Time) override {}
    };

    class TestComponent1Loader1 : public ke::IEntityComponentJsonLoader
    {
    public:
        virtual ke::EntityComponentSptr loadComponent(const ke::json & jsonObject) override
        {
            return std::make_shared<TestComponent1>(nullptr);
        }
    };

    class TestComponent1Loader2 : public ke::IEntityComponentJsonLoader
    {
    public:
        virtual ke::EntityComponentSptr loadComponent(const ke::json & jsonObject) override
        {
            return std::make_shared<TestComponent1>(nullptr);
        }
    };

    class TestComponent2Loader1 : public ke::IEntityComponentJsonLoader
    {
    public:
        virtual ke::EntityComponentSptr loadComponent(const ke::json & jsonObject) override
        {
            return std::make_shared<TestComponent2>(nullptr);
        }
    };

}

TEST_CASE("ke::EntityFactory Component JSON Loader Unit Tests")
{

    SECTION("Test adding component loader for the same component multiple times.")
    {
        ke::EntityFactory factory;
        CHECK(factory.registerComponentJsonLoader<TestComponent1Loader1>(TestComponent1::NAME));

        SECTION("Adding the same loader again for the same component name")
        {
            CHECK(!factory.registerComponentJsonLoader<TestComponent1Loader1>(TestComponent1::NAME));
        }
        
        SECTION("Adding a different loader for the same component name")
        {
            CHECK(!factory.registerComponentJsonLoader<TestComponent1Loader2>(TestComponent1::NAME));
        }

        factory.clear();
    }

    SECTION("Test adding multiple loaders for different components")
    {
        ke::EntityFactory factory;
        CHECK(factory.registerComponentJsonLoader<TestComponent1Loader1>(TestComponent1::NAME));
        CHECK(factory.registerComponentJsonLoader<TestComponent2Loader1>(TestComponent2::NAME));
    }

}

TEST_CASE("ke::EntityFactory Entity Creation Unit Tests")
{
    ke::EntityFactory factory;
    CHECK(factory.registerComponentJsonLoader<TestComponent1Loader1>(TestComponent1::NAME));
    CHECK(factory.registerComponentJsonLoader<TestComponent2Loader1>(TestComponent2::NAME));

    SECTION("Test creating ke::Entity from Entity JSON")
    {
        auto validEntityJson =
        R"(
            {
                "type" : "entity",
                "name" : "TestEntity",
                "components" :
                [
                    {
                        "type" : "entity_component",
                        "component_name" : "TestComponent1"
                    },
                    {
                        "type" : "entity_component",
                        "component_name" : "TestComponent2"
                    }
                ]
            }
        )";

        auto logLevel = ke::Log::instance()->level();
        ke::log::set_level(ke::log::level::debug);
        ke::Log::instance()->debug("Test JSON content: {}", validEntityJson);
        ke::log::set_level(logLevel);

        auto jsonObj = ke::Json::parse(validEntityJson);

        auto entity = factory.createNew(jsonObj);

        CHECK(entity);
        CHECK(entity->getComponentCount() == 2);

        entity->destory();
    }
}

#endif
