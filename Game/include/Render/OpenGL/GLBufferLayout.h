#pragma once

#include <cstdint>
#include <cassert>
#include <string>
#include <vector>

enum class ShaderDataType : uint8_t {
	None,
	Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	UInt, UInt2, UInt3, UInt4,
	Bool
};

static uint8_t ShaderDataTypeSize(const ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:
		return sizeof(float);
	case ShaderDataType::Float2:
		return sizeof(float) * 2;
	case ShaderDataType::Float3:
		return sizeof(float) * 3;
	case ShaderDataType::Float4:
		return sizeof(float) * 4;
	case ShaderDataType::Mat3:
		return sizeof(float) * 3 * 3;
	case ShaderDataType::Mat4:
		return sizeof(float) * 4 * 4;
	case ShaderDataType::Int:
	case ShaderDataType::UInt:
		return sizeof(int);
	case ShaderDataType::Int2:
	case ShaderDataType::UInt2:
		return sizeof(int) * 2;
	case ShaderDataType::Int3:
	case ShaderDataType::UInt3:
		return sizeof(int) * 3;
	case ShaderDataType::Int4:
	case ShaderDataType::UInt4:
		return sizeof(int) * 4;
	case ShaderDataType::Bool:
		return sizeof(bool);
	}

	assert(false && "Unknown shader data type");
	return 0;
}

struct GLBufferElement {
	std::string name;
	ShaderDataType type = ShaderDataType::None;
	uint8_t size = 0;
	uint16_t offset = 0;

	GLBufferElement(ShaderDataType type, const std::string &name) :
		name(name), type(type), size(ShaderDataTypeSize(type)), offset(0) {}

	uint8_t GetComponentCount() const {
		switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Int:
		case ShaderDataType::UInt:
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
		case ShaderDataType::UInt2:
			return 2;
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
		case ShaderDataType::UInt3:
		case ShaderDataType::Mat3:
			return 3;
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
		case ShaderDataType::UInt4:
		case ShaderDataType::Mat4:
			return 4;
		}

		assert(false && "Unknown shader data type");
		return 0;
	}
};

class GLBufferLayout {
public:
	GLBufferLayout() {}
	GLBufferLayout(const std::initializer_list<GLBufferElement> &elements) :
		mElements(elements)
	{
		CalculateOffsetsAndStride();
	}

	const std::vector<GLBufferElement> &GetElements() const { return mElements; }
	uint32_t GetStride() const { return mStride; }

	auto begin() { return std::begin(mElements); }
	auto begin() const { return std::begin(mElements); }
	auto end() { return std::end(mElements); }
	auto end() const { return std::end(mElements); }

private:
	void CalculateOffsetsAndStride() {
		uint32_t offset = 0;
		mStride = 0;
		for (auto &element : mElements) {
			element.offset = offset;
			offset += element.size;
			mStride += element.size;
		}
	}

private:
	std::vector<GLBufferElement> mElements;
	uint32_t mStride = 0;
};
