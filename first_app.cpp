#include "first_app.h"

#include "keyboard_movement_controller.h"
#include "vex_camera.h"
#include "simple_render_system.h"
#include "vex_buffer.h"
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
	// Ubo MEANING: Uniform buffer object
	struct GlobalUbo {
		glm::mat4 projectionView{ 1.f };

		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, 0.2f };
		glm::vec3 lightPosition{ -1.f };
		alignas(16) glm::vec4 lightColor{ 1.f }; // w is light entensity
	};

	FirstApp::FirstApp() {
		globalPool = VexDescriptorPool::Builder(vexDevice)
			.setMaxSets(VexSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VexSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run() {
		std::vector<std::unique_ptr<VexBuffer>> uboBuffers(VexSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<VexBuffer>(
				vexDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT /*| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT*/);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = VexDescriptorSetLayout::Builder(vexDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(VexSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++) {
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			VexDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{
			vexDevice, vexRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		VexCamera camera{};
		//camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = VexGameObject::createGameObject();
		viewerObject.transform.translation.z = -2.5f; // Moved the camera back
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
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.05f, 1000.f);

			if (auto commandBuffer = vexRenderer.beginFrame()) {
				int frameIndex = vexRenderer.getFrameIndex();
				FrameInfo frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				camera,
				globalDescriptorSets[frameIndex]
				};

				// Update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();
				// Render
				vexRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				vexRenderer.endSwapChainRenderPass(commandBuffer);
				vexRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(vexDevice.device());
	}
	// Loads the game models
	void FirstApp::loadGameObjects() {
		std::shared_ptr<VexModel> vexModel =
			VexModel::createModelFromFile(vexDevice, "D:\\Game-Engine\\TestModels\\smooth_vase.obj");

		auto gameObj = VexGameObject::createGameObject();
		gameObj.model = vexModel;
		gameObj.transform.translation = { 0.f, 0.3f, 0.f };
		gameObj.transform.scale = glm::vec3(1.f);
		gameObjects.push_back(std::move(gameObj));

		vexModel = VexModel::createModelFromFile(vexDevice, "D:\\Game-Engine\\TestModels\\floor.obj");
		auto floor = VexGameObject::createGameObject();
		floor.model = vexModel;
		floor.transform.translation = { 0.f, .3f, 0.f };
		floor.transform.scale = { 3.f, 1.f, 3.f };
		gameObjects.push_back(std::move(floor));
	}
}  // namespace lve