#include "HumanView.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/SFML/SfmlEvent.hpp"
#include "KEngine/Log/Log.hpp"

#include <SFML/Window/Event.hpp>

namespace pf
{

    HumanView::HumanView()
    {
        ke::EventManager::registerListener<ke::SfmlEvent>(this, &HumanView::handleWindowEvent);
    }

    HumanView::~HumanView()
    {
        ke::EventManager::deregisterListener<ke::SfmlEvent>(this, &HumanView::handleWindowEvent);
    }

    void HumanView::update(ke::Time elapsedTime)
    {
        // TODO
    }

    void HumanView::handleWindowEvent(ke::EventSptr event)
    {
        std::shared_ptr<ke::SfmlEvent> windowEvent = std::static_pointer_cast<ke::SfmlEvent>(event);
        assert(windowEvent);
        this->handleSfmlEvent(windowEvent->getSfmlEvent());
    }

    void HumanView::handleSfmlEvent(const sf::Event & event)
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            ke::Log::instance()->info("sf::Event::MouseButtonPressed");
            break;

        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                auto echoText = "echo({}): " + testTextBuffer;
                ke::Log::instance()->info(echoText.c_str(), testTextBuffer.length());
                //ke::Log::instance()->info("return pressed");
                //ke::Log::instance()->info(std::string("return pressed").c_str());
                testTextBuffer.clear();
            }
            break;
        }

        case sf::Event::TextEntered:
        {
            if (event.text.unicode < 127 && event.text.unicode > 32) // we only care about printing ASCII characters between 32 and 127.
            {
                testTextBuffer = testTextBuffer + static_cast<char>(event.text.unicode);
            }
            break;
        }        

        default:
            break;
        }
    }
}