#pragma once

#include "vex_camera.h"

//lib
#include <vulkan/vulkan.h>
#include "vex_game_object.h"

namespace vex {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VexCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		VexGameObject::Map& gameObjects;
	};
}
