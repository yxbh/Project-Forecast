#include "KEngine/Core/Keyboard.hpp"

namespace ke
{

    bool Keyboard::isKeyPressed(const ::ke::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(mapKeKeyToInternalSfmlKey(key));
    }

    ::ke::Keyboard::Key Keyboard::mapInternalApiKeyToKeKey(const ::sf::Keyboard::Key sfmlKey)
    {
        return static_cast<::ke::Keyboard::Key>(sfmlKey);
    }

    ::sf::Keyboard::Key Keyboard::mapKeKeyToInternalSfmlKey(const ::ke::Keyboard::Key keKey)
    {
        return static_cast<::sf::Keyboard::Key>(keKey);
    }
}