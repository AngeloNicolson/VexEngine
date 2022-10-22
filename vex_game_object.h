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
		glm::mat4 mat4();
		glm::mat3 normalMatrix();
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