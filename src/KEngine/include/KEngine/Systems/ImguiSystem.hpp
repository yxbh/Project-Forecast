#pragma once

#include "KEngine/Interfaces/ISystem.hpp"

namespace ke
{

    /// <summary>
    /// This system manages ImGui lifetime/update calls.
    /// </summary>
    class ImguiSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(ImguiSystem, 0x55D97EB2)

    public:
        ImguiSystem(void);

        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time elapsedTime) final;

    private:
        ke::SystemUptr systemImpl;
    };

} // namespace ke