#pragma once

#include <memory>

#include "Render/OpenGL/GLBufferLayout.h"

enum class BufferUsage : uint8_t {
	StaticDraw,
	DynamicDraw
};

class GLVertexBuffer {
public:
	GLVertexBuffer(const void* data, uint32_t size, BufferUsage usage);
	~GLVertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, uint32_t size);

	const GLBufferLayout& GetLayout() const { return mLayout; }
	void SetLayout(const GLBufferLayout &layout) { mLayout = layout; }

	static std::shared_ptr<GLVertexBuffer> Create(const void* const vertices, uint32_t size, BufferUsage usage);

private:
	GLBufferLayout mLayout;
	uint32_t mId = 0;
};

class GLIndexBuffer {
public:
	GLIndexBuffer(const uint16_t* indices, uint32_t count, BufferUsage usage);
	~GLIndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return mCount; }

	static std::shared_ptr<GLIndexBuffer> Create(const uint16_t* indices, uint32_t count, BufferUsage usage);

private:
	uint32_t mId = 0;
	uint32_t mCount = 0;
};
