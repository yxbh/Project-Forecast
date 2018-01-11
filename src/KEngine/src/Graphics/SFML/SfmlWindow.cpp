#include "KEngine/Graphics/SFML/SfmlWindow.hpp"

#include <cassert>

namespace ke::sfml
{

    SfmlWindow::SfmlWindow()
    {
    }


    SfmlWindow::~SfmlWindow()
    {
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
        return this->window.get();
    }

    bool SfmlWindow::hasFocus() const
    {
        return this->window->hasFocus();
    }

    void SfmlWindow::requestFocus()
    {
        this->window->requestFocus();
    }

}
