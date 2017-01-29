#pragma once

#include "KEngine/Core/EventManagerImpl.hpp"

#include <memory>

namespace ke
{

    class EventManager
    {
    public:
        static ke::priv::IEventManagerImpl * instance()
        {
            using namespace std;
            using namespace ke::priv;

            static EventManagerUptr instance(new EventManagerImpl);
            return instance.get();
        }

        static void update()
        {

        }
    };

}
