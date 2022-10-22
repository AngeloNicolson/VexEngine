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
	// Loads the game models
	void FirstApp::loadGameObjects() {
		std::shared_ptr<VexModel> vexModel =
			VexModel::createModelFromFile(vexDevice, "D:\\Game-Engine\\TestModels\\colored_cube.obj");

		auto gameObj = VexGameObject::createGameObject();
		gameObj.model = vexModel;
		gameObj.transform.translation = { .0f, .0f, 2.5f };
		gameObj.transform.scale = glm::vec3(0.4f);
		gameObjects.push_back(std::move(gameObj));
	}
}  // namespace lve