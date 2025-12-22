#pragma once

#include "entt/entt.hpp"

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
	entt::registry mRegistry;
	GLFWwindow* mWindow;
};
