#include "render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>

namespace GameEngine
{
  namespace Core
  {
    // This needs to align with Vulkan specification which requires 16 byte padding
    struct SimplePushConstantData
    {
      glm::mat4 transform{1.0f}; // default initialization to identity matrix
      float padding2[2];         // Offset the color to compensate for vec2
      glm::vec3 color;           // TODO: May not need this as we have per vertex coloring
    };

    RenderSystem::RenderSystem(Graphics::VulkanDevice& device, VkRenderPass renderPass) : vulkanDevice{device}
    {
      createPipelineLayout();
      createPipeline(renderPass);
    }

    RenderSystem::~RenderSystem() { vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr); }

    // Pipeline Layout
    void RenderSystem::createPipelineLayout()
    {
      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      pushConstantRange.size = sizeof(SimplePushConstantData);

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{}; // struct

      // Struct member variables
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 0;
      pipelineLayoutInfo.pSetLayouts = nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 1;
      pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

      if(vkCreatePipelineLayout(vulkanDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
          throw std::runtime_error("failed to create pipeline layout!");
        };
    }

    // Pipeline
    void RenderSystem::createPipeline(VkRenderPass renderPass)
    {
      assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

      Graphics::PipelineConfigInfo pipelineConfig{};
      Graphics::GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
      pipelineConfig.renderPass = renderPass;
      pipelineConfig.pipelineLayout = pipelineLayout;
      pipeline = std::make_unique<Graphics::GraphicsPipeline>(vulkanDevice, "Shaders/simple_shader.vert.spv",
                                                              "Shaders/simple_shader.frag.spv", pipelineConfig);
    };

    void RenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<Core::GameObject>& gameObjects)
    {
      pipeline->bind(commandBuffer);

      // Loop over game objects
      for(auto& obj : gameObjects)
        {
          obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.0001f, glm::two_pi<float>());
          obj.transform.rotation.z = glm::mod(obj.transform.rotation.z + 0.0001f, glm::two_pi<float>());

          SimplePushConstantData push{};
          // Order must match the uniform push constant in the shader.vert
          push.color = obj.color;
          push.transform = obj.transform.mat4();

          vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                             0, sizeof(SimplePushConstantData), &push);

          obj.model->bind(commandBuffer);
          obj.model->draw(commandBuffer);
        }
    };

  } // namespace Core
} // namespace GameEngine
