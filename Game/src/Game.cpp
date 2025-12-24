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
	if (GLAD_GL_KHR_debug) {
		LOG_INFO("Setting up OpenGL debug callback...");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
	} else {
		LOG_INFO("Unable to use OpenGL debug callback");
	}
#endif

	QueryRenderSpecs();

	glfwSwapInterval(1);

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	Renderer2D::Init();

	mDirIconTexture = GLTexture::Load("res/textures/DirectoryIcon.png");

	AddColoredRect(glm::vec3(0.0f), glm::vec2(256.0f), glm::vec4(1.0f));
	AddColoredRect(glm::vec3(256.0f, 0.0f, 0.0f), glm::vec2(256.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

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

void Game::QueryRenderSpecs() {
	GLint glMajorVer = 0, glMinorVer = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajorVer);
	glGetIntegerv(GL_MINOR_VERSION, &glMinorVer);
	LOG_INFO("OpenGL Info:");
	LOG_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
	LOG_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
	LOG_INFO("  Version: {0}.{1}", glMajorVer, glMinorVer);

	GLint ctxProfileMask = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &ctxProfileMask);
	if (ctxProfileMask & GL_CONTEXT_CORE_PROFILE_BIT)
		LOG_INFO("  Profile: Core");
	else if (ctxProfileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		LOG_INFO("  Profile: Compat");

	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	std::vector<std::string> infoFlags;
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		infoFlags.emplace_back("Debug");
	if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
		infoFlags.emplace_back("Forward-Compat");

	if (std::size(infoFlags) > 1) {
		std::string flagText = infoFlags[0];
		for (const auto& t : infoFlags) {
			flagText += " | " + t;
		}

		LOG_INFO("  Context Flags: {0}", flagText.c_str());
	} else if (std::size(infoFlags) == 1) {
		LOG_INFO("  Context Flags: {0}", infoFlags[0].c_str());
	}

	GLint maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	LOG_INFO("  Max Texture Size: {0}", maxTextureSize);

	GLint maxRbSize = 0;
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRbSize);
	LOG_INFO("  Max Renderbuffer Size: {0}", maxRbSize);
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
