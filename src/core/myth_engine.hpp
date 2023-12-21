#ifndef MYTH_ENGINE_H
#define MYTH_ENGINE_H

#include "../graphics/myth_window.hpp"

namespace myth_engine {
class engine {

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

private:
  MythWindow mythWindow{WIDTH, HEIGHT, "Hello Vulkan"};
};

} // namespace myth_engine

#endif
