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

        float getAverageFps() const
        {
            auto avgFrameTime = (float)rollingTotal.asMilliseconds() / (float)this->frameTimeHistory.size();
            return 1000.0f / avgFrameTime;
        }

    private:
        ke::Time rollingTotal = ke::Time::Zero;
        std::queue<ke::Time> frameTimeHistory;
    };

}
