#include "GraphicsPipeline.hpp"

#include <cassert>
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

  Graphics::GraphicsPipeline::~GraphicsPipeline()
  {
    // Clean up Vulkan Objects
    vkDestroyShaderModule(vulkanDevice.device(), vertShaderModule, nullptr);
    vkDestroyShaderModule(vulkanDevice.device(), fragShaderModule, nullptr);
    vkDestroyPipeline(vulkanDevice.device(), graphicsPipeline, nullptr);
  }

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
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
    assert(configInfo.renderPass != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");

    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    createShaderModule(vertCode, &vertShaderModule);
    createShaderModule(fragCode, &fragShaderModule);

    VkPipelineShaderStageCreateInfo shaderStages[2];
    // Vertex Shader
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT; // This stage is for vertex shader
    shaderStages[0].module = vertShaderModule;
    shaderStages[0].pName = "main"; // name of entry function in vertex shader
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr; // Mechanism to customize shader functionality

    // Fragment Shader
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT; // This stage is for fragment shader
    shaderStages[1].module = fragShaderModule;
    shaderStages[1].pName = "main"; // name of entry function in fragment shader
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr; // Mechanism to customize shader functionality

    // Struct to describe how we interperet the vertex buffer data that is the initial input in the graphics pipeline
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;

    // Combine viewport and scissor into a single state create info variable
    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1; // Defines the nummber of viewports
    viewportInfo.pViewports = &configInfo.viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &configInfo.scissor;

    // USe all the data above to create the graphics pipeline object
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2; // We only have 2 shaders and only need 2 stages setup so far
    pipelineInfo.pStages = shaderStages;

    // Connect pipeline createInfo to configInfo (Can now be used to create different pipelines)
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    // Can be used to optimize performance
    // Can be less expensive for a GPU to creat a graphics pipeline by deriving from an existing one
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if(vkCreateGraphicsPipelines(vulkanDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) !=
       VK_SUCCESS)
      {
        throw std::runtime_error("Failed to create graphics pipeline");
      }
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

  void Graphics::GraphicsPipeline::bind(VkCommandBuffer commandBuffer)
  {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
  };

  // Creates a basic PipelineConfigInfo struct with default settings for the input assembly stage.
  Graphics::PipelineConfigInfo Graphics::GraphicsPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
  {
    PipelineConfigInfo configInfo{};
    // TODO: NOT SURE THIS SHOULD BE HERE
    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;
    configInfo.multisampleInfo.pSampleMask = nullptr;
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

    // Specify the type of the struct so Vulkan knows how to interpret it.
    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // List - every 3 vertices = 1 triangle
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;              // Disables primitive restart.

    // Configure viewport and scissor
    // The viewport describes the transformation between the pipelines output and target image
    // The viewport tells the pipeline how we want to transform the gl position values to the output image
    configInfo.viewport.x = 0.0f;
    configInfo.viewport.y = 0.0f;
    configInfo.viewport.width = static_cast<float>(width);
    configInfo.viewport.height = static_cast<float>(height);
    configInfo.viewport.minDepth = 0.0f;
    configInfo.viewport.maxDepth = 1.0f;

    // Anything outside is cut (not rasterized).
    // Scissor is essentially a clipping rectangle in framebuffer space.
    configInfo.scissor.offset = {0, 0};
    configInfo.scissor.extent = {width, height};

    // Rasterization - Breaking up geometry into fragments for each triangle the pixel overlaps
    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; // Force z component to be between 0 - 1
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    // For Culling set to -> VK_CULL_MODE_BACK_BIT (vertex's ordered 0 -> 2 -> 1)
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; // Dont cull anything at this stage
    // Determine which face we are looking at by vertex order 0 -> 1 -> 2
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    // Disabled for now
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

    // Color blending to control how to combine colors in the frame buffer
    configInfo.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;                     // Color blending state
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

    /**
     * Depth buffer is an additional attachment to the framebuffer.
     * It stores a depth value (distance from the camera) for every pixel.
     * During rendering, the GPU compares the depth of a new fragment with the current depth value in the depth buffer.
     * If the new fragment is closer (based on depthCompareOp), it updates the depth and possibly the color.
     * This helps ensure that only the closest (visible) fragments are drawn.
     **/
    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;  // Enable depth testing
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE; // Write depth values to the depth buffer
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE; // Disable optional depth bounds test
    configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional (used only if depthBoundsTestEnable is VK_TRUE)
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE; // Disable stencil testing
    configInfo.depthStencilInfo.front = {};                   // Optional; used if stencilTestEnable is VK_TRUE
    configInfo.depthStencilInfo.back = {};                    // Optional

    return configInfo;
  };

} // namespace GameEngine
