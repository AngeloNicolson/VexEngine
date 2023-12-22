#include "myth_pipeline.hpp"

// STD
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace myth_engine {

MythPipeline::MythPipeline(MythEngineDevice &device,
                           const std::string &vertFilepath,
                           const std::string &fragFilepath,
                           const PipelineConfigInfo &configInfo)
    : mythEngineDevice(device) {
  createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
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
void MythPipeline::createGraphicsPipeline(
    const std::string &vertFilepath, const std::string &fragFilepath,
    const PipelineConfigInfo &configInfo) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
}

// Function to create a Vulkan shader module from shader code
void MythPipeline::createShaderModule(const std::vector<char> &code,
                                      VkShaderModule *shaderModule) {
  // Create Vulkan Shader Module Create Info structure
  VkShaderModuleCreateInfo createInfo{};
  // Define the type of structure
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  // Set the size of the shader code
  createInfo.codeSize = code.size();
  // Set the pointer to the shader code
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(mythEngineDevice.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module");
  }
}

PipelineConfigInfo MythPipeline::defaultPipelineConfigInfo(uint32_t width,
                                                           uint32_t height) {
  PipelineConfigInfo configInfo{};

  return configInfo;
}

} // namespace myth_engine
