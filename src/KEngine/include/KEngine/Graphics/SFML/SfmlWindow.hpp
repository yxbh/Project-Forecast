#pragma once

#include "KEngine/Interfaces/IWindow.hpp"
#include "KEngine/Interfaces/IEvent.hpp"

#include <SFML/Graphics.hpp>

namespace ke
{
    class WindowFactory;
}

namespace ke::sfml
{

    using SfWindowUptr = std::unique_ptr<sf::RenderWindow>;
    using SfWindowSptr = std::shared_ptr<sf::RenderWindow>;
    using SfWindowWptr = std::weak_ptr<sf::RenderWindow>;

    class SfmlWindow : public ke::IWindow
    {
        friend class ke::WindowFactory;
    public:
        virtual ~SfmlWindow();

        virtual void display() final;

        virtual ke::String getTitle() const final;

        virtual std::uint32_t getWidth() const final;
        virtual std::uint32_t getHeight() const final;

        virtual void setTitle(const ke::String & title) final;

        virtual bool setThreadCurrent(bool threadCurrent = true) final;

        virtual void * get() final;

    private:
        SfmlWindow();

        SfWindowUptr window;
        ke::String windowTitle;
    };
    
}
