
#pragma once

// TODO: Make Doxygen comments in this file

#include "VulkanDevice.hpp"

// std
#include <vector>
#include <string>

namespace GameEngine
{
  namespace Graphics
  {

    struct PipelineConfigInfo
    {
      PipelineConfigInfo() = default;
      PipelineConfigInfo(const PipelineConfigInfo&) = delete;
      PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

      VkPipelineViewportStateCreateInfo viewportInfo;
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
      VkPipelineRasterizationStateCreateInfo rasterizationInfo;
      VkPipelineMultisampleStateCreateInfo multisampleInfo;
      VkPipelineColorBlendAttachmentState colorBlendAttachment;
      VkPipelineColorBlendStateCreateInfo colorBlendInfo;
      VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
      std::vector<VkDynamicState> dynamicStateEnables;
      VkPipelineDynamicStateCreateInfo dynamicStateInfo;

      VkPipelineLayout pipelineLayout = nullptr;
      VkRenderPass renderPass = nullptr;
      uint32_t subpass = 0;
    };

    class GraphicsPipeline
    {
    public:
      GraphicsPipeline(VulkanDevice& device, const std::string& vertFilepath, const std::string& fragFilepath,
                       const PipelineConfigInfo& configInfo);

      ~GraphicsPipeline();

      GraphicsPipeline(const GraphicsPipeline&) = delete;
      GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

      void bind(VkCommandBuffer commandBuffer);

      // Default static configuration for pipeline config
      static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
      /* @breif Read in a file
       *
       *@param (filepath) reads in a shader file path
       */
      static std::vector<char> readFile(const std::string& filepath);

      // Helper Function
      void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath,
                                  const PipelineConfigInfo& configInfo);

      void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule /*pointer to a pointer*/);

      VulkanDevice& vulkanDevice;
      VkPipeline graphicsPipeline;
      VkShaderModule vertShaderModule;
      VkShaderModule fragShaderModule;
    };
  } // namespace Graphics
} // namespace GameEngine
