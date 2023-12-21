#include "myth_engine.hpp"

namespace myth_engine {

void engine::run() {

  // While window is open poll events like clicks etc.
  while (!mythWindow.shouldClose()) {
    glfwPollEvents();
  }
}

} // namespace myth_engine
