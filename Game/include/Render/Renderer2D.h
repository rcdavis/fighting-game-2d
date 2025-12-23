#pragma once

#include <memory>

#include "glm/glm.hpp"

struct GLFWwindow;
class OrthographicCamera;
class GLTexture;

namespace Renderer2D {
	void Init();
	void Shutdown();

	void BeginScene(const OrthographicCamera& camera);
	void EndScene();

	void Flush();

	void DrawSprite(const glm::mat4& transform, const std::shared_ptr<GLTexture>& texture, const glm::vec4& color);

	void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
}
