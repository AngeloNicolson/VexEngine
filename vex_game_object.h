#pragma once

#include "vex_model.h"

#include <memory>

namespace vex {
	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c,s}, {-s, c} };

			glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
			return rotMatrix * scaleMat;
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
		Transform2dComponent transform2d{};

	private:
		VexGameObject(id_t objId) :id{ objId } {}
		id_t id;
	};
}