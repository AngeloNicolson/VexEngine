#include "render_system.hpp"
#include <vector>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// STD
#include <array>
#include <memory>
#include <stdexcept>

namespace myth_engine
{

struct SimplePushConstantData
{
    glm::mat2 transform{1.f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

RenderSystem::RenderSystem(MythEngineDevice &device, VkRenderPass renderPass) : mythDevice{device}
{
    createPipelineLayout();
    createPipeline(renderPass);
}

RenderSystem::~RenderSystem()
{
    vkDestroyPipelineLayout(mythDevice.device(), pipelineLayout, nullptr);
}

void RenderSystem::createPipelineLayout()
{

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    // offset set to zero because we are combining the vertex and fragment ranges
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(mythDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void RenderSystem::createPipeline(VkRenderPass renderPass)
{
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    MythPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;

    mythPipeline = std::make_unique<MythPipeline>(mythDevice, "graphics/shaders/simple_shader.vert.spv",
                                                  "graphics/shaders/simple_shader.frag.spv", pipelineConfig);
}

void RenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MythGameObject> &gameObjects)
{
    mythPipeline->bind(commandBuffer);

    for (auto &obj : gameObjects)
    {
        int i = 0;
        for (auto &obj : gameObjects)
        {
            i += 1;
            obj.transform2d.rotation =
                glm::mod<float>(obj.transform2d.rotation + 0.000001f * i, 1.0f * glm::two_pi<float>());
        }

        SimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.color = obj.color;
        push.transform = obj.transform2d.mat2();

        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                           sizeof(SimplePushConstantData), &push);

        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}
} // namespace myth_engine
