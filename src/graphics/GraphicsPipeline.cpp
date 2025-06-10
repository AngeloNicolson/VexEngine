#include "GraphicsPipeline.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace GameEngine
{

  Graphics::GraphicsPipeline::GraphicsPipeline(VulkanDevice& device, const std::string& vertFilepath,
                                               const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
      : vulkanDevice(device)
  {
    createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
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
  Graphics::GraphicsPipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath,
                                                     const PipelineConfigInfo& configInfo)
  {
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << "\n";
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << "\n";
  };

  void Graphics::GraphicsPipeline::createShaderModule(const std::vector<char>& code,
                                                      VkShaderModule* shaderModule /*pointer to a pointer*/)
  {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode =
      reinterpret_cast<const uint32_t*>(code.data()); // Vulkan expects a uint32 type instead of a char array

    if(vkCreateShaderModule(vulkanDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
      {
        throw std::runtime_error("Failed to create shader module");
      }
  };

  Graphics::PipelineConfigInfo Graphics::GraphicsPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
  {
    PipelineConfigInfo configInfo{};

    return configInfo;
  };

} // namespace GameEngine
