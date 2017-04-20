#pragma once

namespace ke
{

    /// <summary>
    /// Interface for implementing render commands.
    /// </summary>
    class IRenderCommand
    {
    public:
        virtual ~IRenderCommand() = 0;
    };

    inline IRenderCommand::~IRenderCommand() {}

}