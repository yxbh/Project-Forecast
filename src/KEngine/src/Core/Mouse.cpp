#include "KEngine/Core/Mouse.hpp"

namespace ke
{

    bool Mouse::isButtonPressed(const ::ke::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(::ke::Mouse::mapKeButtonToInternalSfmlButton(button));
    }

    ::ke::Mouse::Button Mouse::mapInternalApiButtonToKeButton(const ::sf::Mouse::Button sfmlButton)
    {
        return static_cast<::ke::Mouse::Button>(sfmlButton);
    }

    ::sf::Mouse::Button Mouse::mapKeButtonToInternalSfmlButton(const ::ke::Mouse::Button keButton)
    {
        return static_cast<::sf::Mouse::Button>(keButton);
    }

}