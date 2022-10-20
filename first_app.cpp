#include "first_app.h"

#include "keyboard_movement_controller.h"
#include "vex_camera.h"
#include "simple_render_system.h"
//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//std
#include <chrono>
#include <stdexcept>
#include <array>
#include <cassert>

namespace vex {
	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{
			vexDevice, vexRenderer.getSwapChainRenderPass() };
		VexCamera camera{};
		//camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = VexGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vexWindow.shouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();

			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(vexWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = vexRenderer.getAspectRataio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			if (auto commandBuffer = vexRenderer.beginFrame()) {
				vexRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				vexRenderer.endSwapChainRenderPass(commandBuffer);
				vexRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(vexDevice.device());
	}
	// temporary helper function, creates a 1x1x1 cube centered at offset
	std::unique_ptr<VexModel> createCubeModel(VexDevice& device, glm::vec3 offset) {
		VexModel::Data modelData{};
		modelData.vertices = {
			// left face (white)
		 {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		 {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		 {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		 {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

		 // right face (yellow)
		 {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		 {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		 {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		 {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

		 // top face (orange, remember y axis points down)
		 {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		 {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		 {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		 {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

		 // bottom face (red)
		 {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		 {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		 {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		 {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

		 // nose face (blue)
		 {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		 {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		 {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		 {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

		 // tail face (green)
		 {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		 {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		 {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		 {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		};
		for (auto& v : modelData.vertices) {
			v.position += offset;
		}

		modelData.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		return std::make_unique<VexModel>(device, modelData);
	}

	void FirstApp::loadGameObjects() {
		std::shared_ptr<VexModel> vexModel = createCubeModel(vexDevice, { .0f, .0f, .0f });

		auto cube = VexGameObject::createGameObject();
		cube.model = vexModel;
		cube.transform.translation = { .0f, .0f, 2.5f };
		cube.transform.scale = { .5f, .5f, .5f };
		gameObjects.push_back(std::move(cube));
	}
}