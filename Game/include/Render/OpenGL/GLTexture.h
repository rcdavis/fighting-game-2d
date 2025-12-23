#pragma once

#include <filesystem>

class GLTexture {
public:
	enum class DataFormat : uint8_t {
		RGB, RGB8,
		RGBA, RGBA8
	};

public:
	GLTexture();
	GLTexture(const std::filesystem::path& filepath);
	~GLTexture();

	uint32_t GetId() const { return mId; }
	uint16_t GetWidth() const { return mWidth; }
	uint16_t GetHeight() const { return mHeight; }

	void Bind(uint8_t slot = 0) const;
	void Unbind() const;

	bool IsLoaded() const { return mIsLoaded; }

	bool operator==(const GLTexture& texture) const;

	static std::shared_ptr<GLTexture> Load(const std::filesystem::path& filepath);

private:
	uint32_t mId = 0;
	uint16_t mWidth = 0;
	uint16_t mHeight = 0;
	DataFormat mInternalFormat = DataFormat::RGBA8;
	DataFormat mDataFormat = DataFormat::RGBA8;
	bool mIsLoaded = false;
};
