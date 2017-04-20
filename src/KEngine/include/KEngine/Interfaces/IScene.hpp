#pragma once

#include <memory>

namespace ke
{

    class Scene;
    using SceneUptr = std::unique_ptr<Scene>;
    using SceneSptr = std::shared_ptr<Scene>;
    using SceneWptr = std::weak_ptr<Scene>;

}