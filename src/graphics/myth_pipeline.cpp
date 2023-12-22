#include "myth_pipeline.hpp"

// STD
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace myth_engine {

MythPipeline::MythPipeline(const std::string &vertFilepath,
                           const std::string &fragFilepath) {
  createGraphicsPipeline(vertFilepath, fragFilepath);
}

std::vector<char> MythPipeline::readFile(const std::string &filepath) {

  // Open the file in binary mode and seek to the end to determine its size
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filepath);
  }

  // Determine the file size by getting the current position (end of file)
  size_t fileSize = static_cast<size_t>(file.tellg());
  // Create a buffer to hold the file contents
  std::vector<char> buffer(fileSize);

  // Setting the read position back to the beginning
  file.seekg(0);

  // Read the file contents into the buffer
  file.read(buffer.data(), fileSize);

  // Close the file
  file.close();

  return buffer;
}
void MythPipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                          const std::string &fragFilepath) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
}

} // namespace myth_engine
