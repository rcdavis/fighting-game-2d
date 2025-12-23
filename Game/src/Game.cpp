#include "Game.h"

#include "Components.h"
#include "Utils/Log.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Render/Renderer2D.h"

static void GlfwErrorCallback(int error, const char* description);
static void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam);

static constexpr uint16_t WindowWidth = 1280;
static constexpr uint16_t WindowHeight = 720;

Game::Game() :
	mCamera(0.0f, WindowWidth, WindowHeight, 0.0f),
	mRegistry(),
	mDirIconTexture(nullptr),
	mWindow(nullptr)
{}

Game::~Game() {
	Shutdown();
}

void Game::Run() {
	if (!Init()) {
		LOG_CRITICAL("Failed to init game");
		return;
	}

	while (!glfwWindowShouldClose(mWindow)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer2D::BeginScene(mCamera);

		mRegistry.view<TransformComponent, ColorComponent>().each([&](const TransformComponent& tc, const ColorComponent& cc) {
			//Renderer2D::DrawQuad(tc.GetTransform(), cc.color);
			Renderer2D::DrawSprite(tc.GetTransform(), mDirIconTexture, cc.color);
		});

		Renderer2D::EndScene();

		glfwSwapBuffers(mWindow);
	}
}

bool Game::Init() {
	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit()) {
		LOG_CRITICAL("Failed to init GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	mWindow = glfwCreateWindow(WindowWidth, WindowHeight, "Deferred Rendering OpenGL", nullptr, nullptr);
	if (!mWindow) {
		LOG_CRITICAL("Failed to create GLFW window");
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		LOG_CRITICAL("Failed to init GLAD");
		return false;
	}

#ifdef DEBUG
	GLint glMajorVer = 0, glMinorVer = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajorVer);
	glGetIntegerv(GL_MINOR_VERSION, &glMinorVer);
	LOG_INFO("OpenGL Info:");
	LOG_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
	LOG_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
	LOG_INFO("  Version: {0}.{1}", glMajorVer, glMinorVer);

	if (GLAD_GL_KHR_debug) {
		LOG_INFO("Setting up OpenGL debug callback...");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
	} else {
		LOG_INFO("Unable to use OpenGL debug callback");
	}
#endif

	glfwSwapInterval(1);

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	Renderer2D::Init();

	mDirIconTexture = GLTexture::Load("res/textures/DirectoryIcon.png");

	AddColoredRect(glm::vec3(0.0f), glm::vec2(256.0f), glm::vec4(1.0f));
	AddColoredRect(glm::vec3(256.0f, 0.0f, 0.0f), glm::vec2(256.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	/*AddColoredRect(glm::vec3(300.0f, 300.0f, 0.0f), glm::vec2(50.0f, 50.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	AddColoredRect(glm::vec3(600.0f, 200.0f, 0.0f), glm::vec2(200.0f, 200.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	AddColoredRect(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(100.0f, 150.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));*/

	return true;
}

void Game::Shutdown() {
	mDirIconTexture = nullptr;

	Renderer2D::Shutdown();

	glfwTerminate();
	mWindow = nullptr;
}

entt::entity Game::AddColoredRect(const glm::vec3& pos, const glm::vec2& scale, const glm::vec4& color) {
	auto e = mRegistry.create();
	mRegistry.emplace<TransformComponent>(e, pos, scale);
	mRegistry.emplace<ColorComponent>(e, color);
	return e;
}

void GlfwErrorCallback(int error, const char* description) {
	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		LOG_CRITICAL("OpenGL Debug Message: {0}", message);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		LOG_ERROR("OpenGL Debug Message: {0}", message);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		LOG_WARN("OpenGL Debug Message: {0}", message);
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		LOG_INFO("OpenGL Debug Message: {0}", message);
		break;

	default:
		LOG_TRACE("OpenGL Debug Message: {0}", message);
		break;
	}
}
