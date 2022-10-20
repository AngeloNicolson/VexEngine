#pragma once

#include "vex_model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace vex {
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		// Will change this to quaternions instead of matrices in the future.
		// Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
		// rotation uses tait-bryan angles order Y1 X2 Z3.
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4() {
			const float c3 = glm::cos(rotation.z);
			const float s3 = glm::sin(rotation.z);
			const float c2 = glm::cos(rotation.x);
			const float s2 = glm::sin(rotation.x);
			const float c1 = glm::cos(rotation.y);
			const float s1 = glm::sin(rotation.y);
			return glm::mat4{
				{
					scale.x * (c1 * c3 + s1 * s2 * s3),
					scale.x * (c2 * s3),
					scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					scale.y * (c3 * s1 * s2 - c1 * s3),
					scale.y * (c2 * c3),
					scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					scale.z * (c2 * s1),
					scale.z * (-s2),
					scale.z * (c1 * c2),
					0.0f,
				},
				{translation.x, translation.y, translation.z, 1.0f} };
		}
	};
	class VexGameObject {
	public:
		using id_t = unsigned int;

		static VexGameObject createGameObject() {
			static id_t currentId = 0;
			return VexGameObject{ currentId++ };
		}

		VexGameObject(const VexGameObject&) = delete;
		VexGameObject& operator=(const VexGameObject&) = delete;
		VexGameObject(VexGameObject&&) = default;
		VexGameObject& operator=(VexGameObject&&) = default;

		const id_t getId() { return id; }

		std::shared_ptr<VexModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		VexGameObject(id_t objId) :id{ objId } {}
		id_t id;
	};
}