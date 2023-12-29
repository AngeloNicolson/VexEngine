#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../core/myth_game_object.hpp"
#include "myth_engine_device.hpp"
#include "myth_pipeline.hpp"
#include <vulkan/vulkan_core.h>

// STD
#include <memory>

namespace myth_engine
{
class RenderSystem
{

  public:
    RenderSystem(MythEngineDevice &device, VkRenderPass renderPass);
    ~RenderSystem();

    RenderSystem(const RenderSystem &) = delete;
    RenderSystem &operator=(const RenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MythGameObject> &gameObjects);

  private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    MythEngineDevice &mythDevice;

    std::unique_ptr<MythPipeline> mythPipeline;
    VkPipelineLayout pipelineLayout;
};

} // namespace myth_engine

#endif
