#include "KEngine/Systems/ImguiSystem.hpp"

#if defined(USE_SFML)
#include "KEngine/Events/SFML/SfmlEvent.hpp"
#endif
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Events/GraphicsLoopFrameEvent.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Log/Log.hpp"

#include "KEngine/Graphics/Imgui.hpp"

#if defined(USE_SFML)
#include <SFML/Graphics/RenderWindow.hpp>
#include "imgui-SFML.h"
#endif

namespace ke::priv
{
    static auto logger = ke::Log::createDefaultLogger("ImguiSystem");

    class ImguiSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(ImguiSystem, 0x341D9615)
        
    public:
        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time elapsedTime) final;

        void handleEvent(ke::EventSptr event);
        void updateImgui(ke::EventSptr event);
        void processSfEvent(ke::EventSptr event);

    private:
        ke::IWindow * mainWindow = nullptr;
    };

    bool ImguiSystem::initialise(void)
    {
        ke::EventManager::registerListener<ke::WindowCreatedEvent>(this, &ImguiSystem::handleEvent);
        ke::EventManager::registerListener<ke::WindowDestroyedEvent>(this, &ImguiSystem::handleEvent);
        ke::EventManager::registerListener<ke::SfmlEvent>(this, &ImguiSystem::processSfEvent);
        ke::EventManager::registerListener<ke::GraphicsLoopFrameEvent>(this, &ImguiSystem::updateImgui);
        
        ImGui::CreateContext();  // create the ImGui context here so we can start manipulating it.

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // enable keyboard navigation.

        return true;
    }

    void ImguiSystem::shutdown(void)
    {
        ImGui::SFML::Shutdown();  // this internally should call `ImGui::DestroyContext()`.

        ke::EventManager::deregisterListener<ke::WindowCreatedEvent>(this, &ImguiSystem::handleEvent);
        ke::EventManager::deregisterListener<ke::WindowDestroyedEvent>(this, &ImguiSystem::handleEvent);
        ke::EventManager::deregisterListener<ke::SfmlEvent>(this, &ImguiSystem::processSfEvent);
        ke::EventManager::deregisterListener<ke::GraphicsLoopFrameEvent>(this, &ImguiSystem::updateImgui);
    }

    void ImguiSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    void ImguiSystem::updateImgui(ke::EventSptr event)
    {
        // NOTE: This function should only be called from the render/graphics loop or thread.

        switch (event->getType())
        {
        case ke::GraphicsLoopFrameEvent::TYPE:
        {
#if defined(USE_SFML)
            auto frameEvent = std::static_pointer_cast<ke::GraphicsLoopFrameEvent>(event);
            if (this->mainWindow)
            {
                auto sfWindow = static_cast<sf::RenderWindow*>(mainWindow->get());
                ImGui::SFML::Update(*sfWindow, sf::microseconds(frameEvent->getFrameTimeSpan().asMicroseconds()));
            }
#endif
            break;
        }
        default:
            logger->warn("ImguiSystem::updateImgui(): Unhandled event: {}", event->getName());
            break;
        }
    }

    void ImguiSystem::processSfEvent(ke::EventSptr event)
    {
        // NOTE: This function should only be called from the render/graphics loop or thread.

        switch (event->getType())
        {
#if defined(USE_SFML)
        case ke::SfmlEvent::TYPE:
        {
            auto keSfEvent = std::static_pointer_cast<ke::SfmlEvent>(event);
            ImGui::SFML::ProcessEvent(keSfEvent->getSfmlEvent());
            break;
        }
#endif
        default:
            logger->warn("ImguiSystem::processSfEvent(): Unhandled event: {}", event->getName());
            break;
        }
    }

    void ImguiSystem::handleEvent(ke::EventSptr event)
    {
        switch (event->getType())
        {
        case ke::WindowCreatedEvent::TYPE:
        {
            auto windowEvent = std::static_pointer_cast<ke::WindowCreatedEvent>(event);
            this->mainWindow = windowEvent->get();
            break;
        }
        case ke::WindowDestroyedEvent::TYPE:
        {
            this->mainWindow = nullptr;
            break;
        }
        default:
            logger->warn("ImguiSystem: Unhandled event: {}", event->getName());
            break;
        }
    }
} // ns ke::priv


namespace ke
{
    ImguiSystem::ImguiSystem(void)
    {
        this->systemImpl = std::make_unique<ke::priv::ImguiSystem>();
    }

    bool ImguiSystem::initialise(void)
    {
        return this->systemImpl->initialise();
    }

    void ImguiSystem::shutdown(void)
    {
        this->systemImpl->shutdown();
    }

    void ImguiSystem::update(ke::Time elapsedTime)
    {
        this->systemImpl->update(elapsedTime);
    }

} // namespace ke