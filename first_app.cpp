#include "first_app.h"
#include "simple_render_system.h"
//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//std
#include <stdexcept>
#include <array>
#include <cassert>

namespace vex {
	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ vexDevice, vexRenderer.getSwapChainRenderPass() };
		while (!vexWindow.shouldClose()) {
			glfwPollEvents();

			if (auto commandBuffer = vexRenderer.beginFrame()) {
				vexRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				vexRenderer.endSwapChainRenderPass(commandBuffer);
				vexRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(vexDevice.device());
	}

	void FirstApp::loadGameObjects() {
		std::vector<VexModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f }, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		auto vexModel = std::make_shared<VexModel>(vexDevice, vertices);

		auto triangle = VexGameObject::createGameObject();
		triangle.model = vexModel;
		triangle.color = { .1f, .0f, .1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}