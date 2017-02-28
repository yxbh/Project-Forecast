#pragma once

#include "KEngine/Common/Time.hpp"

#include <queue>

namespace ke
{


    class FrameRateCounter
    {
    public:

        FrameRateCounter(uint32_t frameHistorySize)
        {
            for (uint32_t i = 0; i < frameHistorySize; ++i) this->frameTimeHistory.push(ke::Time::Zero);
        }

        void push(ke::Time elapsedFrameTime)
        {
            rollingTotal -= this->frameTimeHistory.front();
            rollingTotal += elapsedFrameTime;
            this->frameTimeHistory.pop();
            this->frameTimeHistory.push(elapsedFrameTime);
        }

        double getAverageFps() const
        {
            auto avgFrameTime = rollingTotal.asSeconds() / (double)this->frameTimeHistory.size();
            return 1.0 / avgFrameTime;
        }

    private:
        ke::Time rollingTotal = ke::Time::Zero;
        std::queue<ke::Time> frameTimeHistory;
    };

}
