#include "KEngine/Graphics/SFML/SfmlWindow.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/GraphicsLoopFrameEvent.hpp"

#include <cassert>

namespace ke::sfml
{

    SfmlWindow::SfmlWindow()
    {
        ke::EventManager::registerListener<ke::GraphicsLoopFrameEvent>(
            this, &ke::sfml::SfmlWindow::handleGraphicsLoopFrameEvent);
    }


    SfmlWindow::~SfmlWindow()
    {
        if (window)
        {
            ke::EventManager::deregisterListener<ke::GraphicsLoopFrameEvent>(
                this, &ke::sfml::SfmlWindow::handleGraphicsLoopFrameEvent);
        }
    }

    void SfmlWindow::display()
    {
        this->window->display();
    }

    ke::String SfmlWindow::getTitle() const
    {
        return this->windowTitle;
    }

    std::uint32_t SfmlWindow::getWidth() const
    {
        return this->window->getSize().x;
    }

    std::uint32_t SfmlWindow::getHeight() const
    {
        return this->window->getSize().y;
    }

    void SfmlWindow::setTitle(const ke::String & p_title)
    {
        this->windowTitle = p_title;
        this->window->setTitle(p_title);
    }

    bool SfmlWindow::setThreadCurrent(bool threadCurrent)
    {
        return this->window->setActive(threadCurrent);
    }

    void * SfmlWindow::get()
    {
        return window.get();
    }

    void SfmlWindow::handleGraphicsLoopFrameEvent(ke::EventSptr event)
    {
        std::shared_ptr<GraphicsLoopFrameEvent> frameEvent = std::static_pointer_cast<GraphicsLoopFrameEvent>(event);
        if (frameEvent)
        {
            this->setTitle("KEngine - " + std::to_string(frameEvent->getFrameTimeSpan().asMilliseconds()) + "ms/frame");
        }
    }

}