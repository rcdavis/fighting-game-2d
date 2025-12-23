#include "Render/OpenGL/GLBuffer.h"

#include "glad/gl.h"

static uint32_t BufferUsageToGLType(const BufferUsage usage) {
	if (usage == BufferUsage::StaticDraw)
		return GL_STATIC_DRAW;

	return GL_DYNAMIC_DRAW;
}

GLVertexBuffer::GLVertexBuffer(const void* data, uint32_t size, BufferUsage usage) {
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToGLType(usage));
}

GLVertexBuffer::~GLVertexBuffer() {
	glDeleteBuffers(1, &mId);
}

void GLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void GLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::SetData(const void* data, uint32_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

std::shared_ptr<GLVertexBuffer> GLVertexBuffer::Create(const void* data, uint32_t size, BufferUsage usage) {
	return std::make_shared<GLVertexBuffer>(data, size, usage);
}

GLIndexBuffer::GLIndexBuffer(const uint16_t* indices, uint32_t count, BufferUsage usage) :
	mCount(count)
{
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * count, indices, BufferUsageToGLType(usage));
}

GLIndexBuffer::~GLIndexBuffer() {
	glDeleteBuffers(1, &mId);
}

void GLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}

void GLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::shared_ptr<GLIndexBuffer> GLIndexBuffer::Create(const unsigned short* indices, unsigned int count, BufferUsage usage) {
	return std::make_shared<GLIndexBuffer>(indices, count, usage);
}
