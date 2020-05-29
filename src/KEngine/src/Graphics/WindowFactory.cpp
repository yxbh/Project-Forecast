#include "KEngine/Graphics/WindowFactory.hpp"

#include "KEngine/Graphics/SFML/SfmlWindow.hpp"

#include "KEngine/Log/Log.hpp"

#include "KEngine/Common/macros.hpp"

namespace ke
{

    ke::WindowSptr WindowFactory::newWindow(
        unsigned p_width, unsigned p_height,
        int p_posX, int p_posY,
        const ke::String & p_title)
    {
        return createNewSfmlWindow(p_width, p_height, p_posX, p_posY, p_title);
    }

    ke::WindowSptr WindowFactory::createNewSfmlWindow(
        unsigned p_width, unsigned p_height,
        int p_posX, int p_posY,
        const ke::String & p_title)
    {
#if defined(USE_SFML)
        auto window = std::shared_ptr<ke::sfml::SfmlWindow>(new ke::sfml::SfmlWindow);
        assert(window);
        window->window = ke::sfml::SfWindowUptr(
            new sf::RenderWindow(
                sf::VideoMode(p_width, p_height),
                p_title.c_str(),
                sf::Style::Default
            )
        );
        if (p_posX != 0 && p_posY != 0)  // TODO: temporarily ignore (0,0) until I need to figure out if that is a sensible coordinate to set.
        {
            window->window->setPosition({ p_posX, p_posY });
        }
        return window;
#else
        KE_UNUSED(p_width); KE_UNUSED(p_height);
        KE_UNUSED(p_posX); KE_UNUSED(p_posY);
        KE_UNUSED(p_title);
        return nullptr;
#endif
    }

}