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

private:
	OrthographicCamera mCamera;
	entt::registry mRegistry;
	GLFWwindow* mWindow;
};
