//===-- myth_pipeline.hpp -------------------------------------------------===//
//
// Part of the Myth Engine, a Vulkan-based graphics engine.
//
// This file defines the MythPipeline class, managing the creation and
// configuration of a graphics pipeline.
//
//===----------------------------------------------------------------------===//

#ifndef MYTH_PIPELINE_H
#define MYTH_PIPELINE_H

#include "myth_engine_device.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace myth_engine {

/**
 * @brief Struct used to organize data to configure the pipeline.
 *
 * This struct is separated from the pipeline class to allow easy configuration
 * at the application layer and to facilitate sharing configuration between
 * multiple pipelines.
 */
struct PipelineConfigInfo {
  // Values stating how the different stages of the pipeline will work
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

/**
 * @brief Handles the creation and management of a graphics pipeline.
 */
class MythPipeline {

public:
  /**
   * @brief Constructs a MythPipeline with provided file paths for vertex and
   * fragment shaders.
   * @param vertFilepath The file path for the vertex shader.
   * @param fragFilepath The file path for the fragment shader.
   */
  MythPipeline(MythEngineDevice &device, const std::string &vertFilepath,
               const std::string &fragFilepath,
               const PipelineConfigInfo &configInfo);

  ~MythPipeline();

  // Copy constructor - Disabled to prevent copying of MythPipeline objects
  MythPipeline(const MythPipeline &) = delete;
  // Deleted copy assignment operator - Prevents assignment of MythPipeline
  void operator=(const MythPipeline &) = delete;

  /**
   * @brief Binds a Vulkan command buffer.
   *
   * Binds the specified Vulkan command buffer for use.
   * @param commandBuffer The Vulkan command buffer to be bound.
   */
  void bind(VkCommandBuffer commandBuffer);

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

private:
  /**
   * @brief Reads the contents of a file and returns them as a vector of
   * characters.
   * @param filepath The file path of the file to be read.
   * @return The contents of the file as a vector of characters.
   */
  static std::vector<char> readFile(const std::string &filepath);

  /**
   * @brief Creates the graphics pipeline using the specified vertex and
   * fragment shader file paths.
   * @param vertFilepath The file path for the vertex shader.
   * @param fragFilepath The file path for the fragment shader.
   */
  void createGraphicsPipeline(const std::string &vertFilepath,
                              const std::string &fragFilepath,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(const std::vector<char> &code,
                          VkShaderModule *shaderModule);

  // Under other circumstances this could cause a dangling pointer, but since
  // the pipeline cannot exist without device then this will be ok
  MythEngineDevice &mythEngineDevice;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};

} // namespace myth_engine

#endif
