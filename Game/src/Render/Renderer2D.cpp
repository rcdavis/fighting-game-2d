#include "Render/Renderer2D.h"

#include <vector>

#include "Render/OrthographicCamera.h"
#include "Render/OpenGL/GLBuffer.h"
#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLTexture.h"
#include "Render/OpenGL/GLVertexArray.h"

#include "glm/gtc/matrix_transform.hpp"

#include "glad/gl.h"

namespace Renderer2D {
	struct QuadVertex {
		glm::vec3 position{0.0f};
		glm::vec4 color{1.0f};
		glm::vec2 texCoord{0.0f};
		int32_t texIndex = 0;
	};

	struct Renderer2DData {
		static constexpr uint32_t MaxQuads = 2'000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;

		static constexpr std::array<glm::vec4, 4> quadVertexPositions = {
			glm::vec4 { 0.0f, 0.0f, 0.0f, 1.0f },
			glm::vec4 { 1.0f, 0.0f, 0.0f, 1.0f },
			glm::vec4 { 1.0f, 1.0f, 0.0f, 1.0f },
			glm::vec4 { 0.0f, 1.0f, 0.0f, 1.0f }
		};

		std::shared_ptr<GLVertexArray> quadVao;
		std::shared_ptr<GLVertexBuffer> quadVb;
		std::shared_ptr<GLShader> quadShader;
		std::shared_ptr<GLShader> quadTexturedShader;

		std::array<std::shared_ptr<GLTexture>, MaxTextureSlots> textureSlots{};

		std::vector<QuadVertex> quadVertexBufferBase;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32_t quadIndexCount = 0;
		uint32_t textureSlotIndex = 0;
	};

	static Renderer2DData s_Data;

	void Init() {
		s_Data.quadVao = GLVertexArray::Create();

		s_Data.quadVb = GLVertexBuffer::Create(nullptr, s_Data.MaxVertices * sizeof(QuadVertex), BufferUsage::DynamicDraw);
		s_Data.quadVb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Int, "a_TexIndex" }
		});
		s_Data.quadVao->AddVertexBuffer(s_Data.quadVb);

		s_Data.quadVertexBufferBase.resize(s_Data.MaxVertices);
		std::vector<uint16_t> quadIndices(s_Data.MaxIndices);

		uint16_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset;

			offset += 4;
		}

		auto quadIb = GLIndexBuffer::Create(std::data(quadIndices), s_Data.MaxIndices, BufferUsage::StaticDraw);
		s_Data.quadVao->SetIndexBuffer(quadIb);

		s_Data.quadShader = GLShader::Create("Colored", "res/shaders/colored.vs", "res/shaders/colored.fs");
		s_Data.quadTexturedShader = GLShader::Create("Textured", "res/shaders/textured.vs", "res/shaders/textured.fs");
	}

	void Shutdown() {
		s_Data.textureSlots.fill(nullptr);

		s_Data.quadVertexBufferPtr = nullptr;
		s_Data.quadVertexBufferBase.clear();

		s_Data.quadShader = nullptr;
		s_Data.quadTexturedShader = nullptr;
		s_Data.quadVb = nullptr;
		s_Data.quadVao = nullptr;
	}

	void BeginScene(const OrthographicCamera& camera) {
		s_Data.quadVao->Bind();
		//s_Data.quadShader->Bind();
		//s_Data.quadShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.quadTexturedShader->Bind();
		s_Data.quadTexturedShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = std::data(s_Data.quadVertexBufferBase);
	}

	void EndScene() {
		const uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)std::data(s_Data.quadVertexBufferBase));
		s_Data.quadVb->SetData(std::data(s_Data.quadVertexBufferBase), dataSize);

		Flush();
	}

	void Flush() {
		if (s_Data.quadIndexCount == 0)
			return;

		for (uint32_t i = 0; i < s_Data.textureSlotIndex; ++i)
			s_Data.textureSlots[i]->Bind(i);

		//s_Data.quadTexturedShader->Bind();
		glDrawElements(GL_TRIANGLES, s_Data.quadIndexCount, GL_UNSIGNED_SHORT, nullptr);

		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = std::data(s_Data.quadVertexBufferBase);
	}

	void DrawSprite(const glm::mat4& transform, const std::shared_ptr<GLTexture>& texture, const glm::vec4& color) {
		if (s_Data.quadIndexCount >= s_Data.MaxIndices)
			Flush();

		constexpr std::array<glm::vec2, 4> texCoords = {
			glm::vec2 { 0.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f },
			glm::vec2 { 1.0f, 1.0f }, glm::vec2 { 0.0f, 1.0f }
		};

		uint32_t texIndex = -1;
		for (uint32_t i = 0; i < s_Data.textureSlotIndex; ++i) {
			if (*s_Data.textureSlots[i] == *texture) {
				texIndex = i;
				break;
			}
		}

		if (texIndex == -1) {
			texIndex = s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex++] = texture;
		}

		for (uint32_t i = 0; i < std::size(texCoords); ++i) {
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = texCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = texIndex;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
	}

	void DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
		if (s_Data.quadIndexCount >= s_Data.MaxIndices)
			Flush();

		constexpr std::array<glm::vec2, 4> texCoords = {
			glm::vec2 { 0.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f },
			glm::vec2 { 1.0f, 1.0f }, glm::vec2 { 0.0f, 1.0f }
		};

		for (uint32_t i = 0; i < std::size(texCoords); ++i) {
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = texCoords[i];
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
	}
}
