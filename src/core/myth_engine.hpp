#ifndef MYTH_ENGINE_H
#define MYTH_ENGINE_H

#include "../graphics/myth_pipeline.hpp"
#include "../graphics/myth_window.hpp"

namespace myth_engine {
class engine {

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

private:
  MythWindow mythWindow{WIDTH, HEIGHT, "Hello Vulkan"};
  MythPipeline mythPipeline{"graphics/shaders/simple_shader.vert.spv",
                            "graphics/shaders/simple_shader.frag.spv"};
};

} // namespace myth_engine

#endif
