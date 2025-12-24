#pragma once

#include <memory>

#include "entt/entt.hpp"
#include "Render/OrthographicCamera.h"
#include "Render/OpenGL/GLTexture.h"

struct GLFWwindow;

class Game {
public:
	Game();
	~Game();

	void Run();

private:
	bool Init();
	void Shutdown();

	entt::entity AddColoredRect(const glm::vec3& pos, const glm::vec2& scale, const glm::vec4& color);

	void QueryRenderSpecs();

private:
	OrthographicCamera mCamera;
	entt::registry mRegistry;
	std::shared_ptr<GLTexture> mDirIconTexture;
	GLFWwindow* mWindow;
};
