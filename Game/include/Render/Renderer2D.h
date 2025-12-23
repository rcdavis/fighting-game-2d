#pragma once

#include "glm/glm.hpp"

struct GLFWwindow;
class OrthographicCamera;

namespace Renderer2D {
	void Init();
	void Shutdown();

	void BeginScene(const OrthographicCamera& camera);
	void EndScene();

	void Flush();

	void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
}
