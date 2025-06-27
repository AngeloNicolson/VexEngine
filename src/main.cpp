
#include <iostream>
#include "./core/application.hpp"

// std
#include <cstdlib>
#include <stdexcept>

int main()
{
  GameEngine::Core::Application app{};

  try
    {
      app.run();
    }
  catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
