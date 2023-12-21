#include "./core/myth_engine.hpp"

// STD
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {

  // Instance of engine
  myth_engine::engine engine{};

  try {
    engine.run();
  } catch (const std::exception &error) {
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

  return 0;
}
