#pragma once

#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Views/HumanView.hpp"

namespace sf
{
    class Event;
}

namespace pf
{
    
    class HumanView : public ke::HumanView
    {
    public:
        HumanView();
        virtual ~HumanView();

        virtual void update(ke::Time elapsedTime) override;

    private:
        std::string testTextBuffer;

        void handleWindowEvent(ke::EventSptr event);
        void handleSfmlEvent(const sf::Event & event);
    };

}