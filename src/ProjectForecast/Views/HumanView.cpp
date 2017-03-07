#include "HumanView.hpp"

#include "../InputControllers/InputControllers.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/SFML/SfmlEvent.hpp"
#include "KEngine/Log/Log.hpp"

#include <SFML/Window/Event.hpp>

namespace pf
{

    HumanView::HumanView()
    {
        mouseController = std::make_unique<pf::MouseInputController>();
        keyboardController = std::make_unique<pf::KeyboardInputController>();

        scene = std::make_unique<ke::Scene>();

        ke::EventManager::registerListener<ke::SfmlEvent>(this, &HumanView::handleWindowEvent);
    }

    HumanView::~HumanView()
    {
        ke::EventManager::deregisterListener<ke::SfmlEvent>(this, &HumanView::handleWindowEvent);
    }

    void HumanView::attachEntity(ke::EntityId entityId)
    {
        this->ke::HumanView::attachEntity(entityId);
        static_cast<pf::KeyboardInputController*>(this->keyboardController.get())->attachEntity(entityId);
    }

    void HumanView::update(ke::Time elapsedTime)
    {
        assert(mouseController);
        assert(keyboardController);
        mouseController->update(elapsedTime);
        keyboardController->update(elapsedTime);
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
        {

            mouseController->onButtonPressed(
                ke::Mouse::mapInternalApiButtonToKeButton(event.mouseButton.button),
                { event.mouseButton.x, event.mouseButton.y });
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            mouseController->onButtonReleased(
                ke::Mouse::mapInternalApiButtonToKeButton(event.mouseButton.button),
                { event.mouseButton.x, event.mouseButton.y });
            break;
        }

        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                ke::Log::instance()->info("echo({}): {}", testTextBuffer.length(), testTextBuffer);
                testTextBuffer.clear();
            }
            break;
        }

        case sf::Event::TextEntered:
        {
            if (event.text.unicode >= 32 && event.text.unicode < 127) // we only care about printing ASCII characters between 32 and 127.
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
