#ifndef MYTH_ENGINE_H
#define MYTH_ENGINE_H

#include "../graphics/myth_engine_device.hpp"
#include "../graphics/myth_window.hpp"
#include "myth_game_object.hpp"
#include "myth_renderer.hpp"

// STD
#include <vector>

namespace myth_engine
{
class Engine
{

  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Engine();
    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    void run();

  private:
    void loadGameObjects();

    MythWindow mythWindow{WIDTH, HEIGHT, "Myth Engine"};
    MythEngineDevice mythDevice{mythWindow};
    MythRenderer mythRenderer{mythWindow, mythDevice};

    std::vector<MythGameObject> gameObjects;
};

} // namespace myth_engine

#endif
