#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Core/EntityManager.hpp"
#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/Json.hpp"

#include "KEngine/UnitTest/catch.hpp"

#include <memory>

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

    class TestComponent1Loader1 : public ke::IEntityComponentBuilder
    {
    public:
        virtual ke::EntityComponentSptr build(const std::any &) override
        {
            return std::make_shared<TestComponent1>(nullptr);
        }
    };

    class TestComponent1Loader2 : public ke::IEntityComponentBuilder
    {
    public:
        virtual ke::EntityComponentSptr build(const std::any &) override
        {
            return std::make_shared<TestComponent1>(nullptr);
        }
    };

    class TestComponent2Loader1 : public ke::IEntityComponentBuilder
    {
    public:
        virtual ke::EntityComponentSptr build(const std::any &) override
        {
            return std::make_shared<TestComponent2>(nullptr);
        }
    };

    ke::EntityManager em;

    class TestEntityBuilder : public ke::IEntityBuilder
    {
    public:
        using IEntityBuilder::IEntityBuilder;

        virtual ke::Entity* build(const std::any& p_parameters) final
        {
            auto jsonParms = std::any_cast<const ke::Json&>(p_parameters);
            auto entity = em.newEntity(em.newId()).lock();

            for (auto& componentJson : jsonParms["components"])
            {
                if (componentJson["component_name"] == "TestComponent1")
                {
                    auto component = ke::makeEntityComponent<TestComponent1>(entity);
                    entity->addComponent(component);
                }
                else if (componentJson["component_name"] == "TestComponent2")
                {
                    auto component = ke::makeEntityComponent<TestComponent2>(entity);
                    entity->addComponent(component);
                }
            }

            return entity.get();
        }
    };
}

TEST_CASE("ke::EntityFactory Component JSON Loader Unit Tests")
{

    SECTION("Test adding component loader for the same component multiple times.")
    {
        ke::EntityFactory factory;
        REQUIRE(factory.registerComponentBuilder<TestComponent1Loader1>(TestComponent1::NAME));

        SECTION("Adding the same loader again for the same component name")
        {
            CHECK(!factory.registerComponentBuilder<TestComponent1Loader1>(TestComponent1::NAME));
        }
        
        SECTION("Adding a different loader for the same component name")
        {
            CHECK(!factory.registerComponentBuilder<TestComponent1Loader2>(TestComponent1::NAME));
        }

        factory.clear();
    }

    SECTION("Test adding multiple loaders for different components")
    {
        ke::EntityFactory factory;
        REQUIRE(factory.registerComponentBuilder<TestComponent1Loader1>(TestComponent1::NAME));
        REQUIRE(factory.registerComponentBuilder<TestComponent2Loader1>(TestComponent2::NAME));
    }

}

TEST_CASE("ke::EntityFactory Entity Creation Unit Tests")
{
    ke::EntityFactory factory;
    REQUIRE(factory.registerEntityBuilder("TestEntity", std::make_unique<TestEntityBuilder>(&factory)));
    REQUIRE(factory.registerComponentBuilder<TestComponent1Loader1>(TestComponent1::NAME));
    REQUIRE(factory.registerComponentBuilder<TestComponent2Loader1>(TestComponent2::NAME));

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

        auto entity = factory.createNew("TestEntity", jsonObj);

        REQUIRE(entity);
        REQUIRE(entity->getComponentCount() == 2);

        entity->destory();
    }
}

#endif
