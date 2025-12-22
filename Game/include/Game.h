#pragma once

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
	GLFWwindow* mWindow;
};
