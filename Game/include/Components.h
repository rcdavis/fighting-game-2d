#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct TransformComponent {
	glm::vec3 position{0.0f};
	glm::vec3 scale{1.0f};

	glm::mat4 GetTransform() const {
		return glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);
	}
};

struct ColorComponent {
	glm::vec4 color {1.0f};
};
