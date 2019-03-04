#include "KEngine/Interfaces/IEntityBuilder.hpp"

namespace ke
{
    class JsonEntityBuilder final : public IEntityBuilder
    {
    public:
        using IEntityBuilder::IEntityBuilder;

        virtual EntitySptr create(const std::any & p_parameters) final;
    };
}