#include "GraphicsPipeline.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace GameEngine
{

  Graphics::GraphicsPipeline::GraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath)
  {
    createGraphicsPipeline(vertFilepath, fragFilepath);
  };

  std::vector<char> Graphics::GraphicsPipeline::readFile(const std::string& filepath)
  {
    // first bitflag means when file is open seek to end to get size (Makes it easier to get file size)
    // binary reads it in as binary to avvoid text transfermations
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if(!file.is_open()) { throw std::runtime_error("Failed to open file: " + filepath); }

    // Already at end of file so return the current position of the input stream's "get" pointer.
    size_t fileSize = static_cast<size_t>(file.tellg());

    std::vector<char> buffer(fileSize);

    // Seek to start of file so we can now read the data
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
  };

  // Creat graphics pipeline helper function
  void
  Graphics::GraphicsPipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath)
  {
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << "\n";
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << "\n";
  };

} // namespace GameEngine
