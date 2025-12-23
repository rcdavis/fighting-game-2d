#pragma once

#include "entt/entt.hpp"
#include "Render/OrthographicCamera.h"

struct GLFWwindow;

class Game {
public:
	Game();
	~Game();

	void Run();

private:
	bool Init();
	void Shutdown();

	entt::entity AddColoredRect(const glm::vec3& pos, const glm::vec3& scale, const glm::vec4& color);

private:
	OrthographicCamera mCamera;
	entt::registry mRegistry;
	GLFWwindow* mWindow;
};
