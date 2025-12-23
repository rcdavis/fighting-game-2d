#include "Render/Renderer2D.h"

namespace Renderer2D {
	struct QuadVertex {
		glm::vec3 position{0.0f};
		glm::vec4 color{1.0f};
		glm::vec2 texCoord{0.0f};
	};

	void Init() {

	}

	void Shutdown() {

	}

	void BeginScene(const OrthographicCamera& camera) {

	}

	void EndScene() {

	}

	void Flush() {

	}

	void DrawQuad(const glm::mat4& transform, const glm::vec4& color) {

	}
}
