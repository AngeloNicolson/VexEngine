#include "myth_engine.hpp"
#include "../graphics/render_system.hpp"
#include "myth_game_object.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// STD
#include <memory>

namespace myth_engine
{

Engine::Engine()
{
    loadGameObjects();
}

Engine::~Engine()
{
}

void Engine::run()
{
    RenderSystem renderSystem{mythDevice, mythRenderer.getSwapChainRenderPass()};

    // While window is open poll events like clicks etc.
    while (!mythWindow.shouldClose())
    {
        glfwPollEvents();

        if (auto commandBuffer = mythRenderer.beginFrame())
        {

            mythRenderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderGameObjects(commandBuffer, gameObjects);
            mythRenderer.endSwapChainRenderPass(commandBuffer);
            mythRenderer.endFrame();
        }
    }

    // Waits untill all gpu operations have completed.
    vkDeviceWaitIdle(mythDevice.device());
}

void Engine::loadGameObjects()
{
    std::vector<MythVertexBufferManager::Vertex> vertices{

        {{0.0f, -2.0f}, {1.0f, 0.0f, 0.0f}}, {{-0.01, 0.0f}, {0.0f, 1.0f, 0.0f}},  {{0.1f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 2.0f}, {0.0f, 1.0f, 0.0f}},  {{0.01f, -0.0f}, {0.0f, 0.0f, 1.0f}}, {{-0.1f, 1.0f}, {1.0f, 0.0f, 0.0f}}};
    // for (const auto &vertex : vertices) {
    //   std::cout << "Vertex Position: (" << vertex.position.x << ", "
    //             << vertex.position.y << ")" << std::endl;
    // }
    auto mythVertexBuffer = std::make_shared<MythVertexBufferManager>(mythDevice, vertices);

    std::vector<glm::vec3> colors{
        {1.0f, 0.1f, 0.0f}, {1.0f, 0.2f, 0.0f}, {1.0f, 0.3f, 0.0f}, {1.0f, 0.4f, 0.0f}, {1.0f, 0.5f, 0.0f} //
    };
    for (auto &color : colors)
    {
        color = glm::pow(color, glm::vec3{2.2f});
    }
    for (int i = 0; i < 100; i++)
    {
        auto triangle = MythGameObject::createGameObject();
        triangle.model = mythVertexBuffer;
        triangle.transform2d.scale = glm::vec2(.4f) + i * 0.015f;
        triangle.transform2d.rotation = i * glm::pi<float>() * .015f;
        triangle.color = colors[i % colors.size()];
        gameObjects.push_back(std::move(triangle));
    }
}

} // namespace myth_engine
