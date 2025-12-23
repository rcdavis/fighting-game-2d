#include "Render/OpenGL/GLTexture.h"

#include "glad/gl.h"
#include "stb_image.h"

#include "Utils/Log.h"

static uint16_t DataFormatToGLType(const GLTexture::DataFormat format) {
	switch (format) {
	case GLTexture::DataFormat::RGB:
		return GL_RGB;
	case GLTexture::DataFormat::RGB8:
		return GL_RGB8;
	case GLTexture::DataFormat::RGBA:
		return GL_RGBA;
	case GLTexture::DataFormat::RGBA8:
		return GL_RGBA8;
	}

	return GL_RGBA8;
}

GLTexture::GLTexture() {
	glGenTextures(1, &mId);
}

GLTexture::GLTexture(const std::filesystem::path& filepath) {
	int width, height, channels;
	stbi_uc* const data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
	if (!data) {
		LOG_ERROR("Failed to load image {0}", filepath.c_str());
		return;
	}

	mIsLoaded = true;

	mWidth = (uint16_t)width;
	mHeight = (uint16_t)height;

	if (channels == 4) {
		mInternalFormat = DataFormat::RGBA8;
		mDataFormat = DataFormat::RGBA;
	} else if (channels == 3) {
		mInternalFormat = DataFormat::RGB8;
		mDataFormat = DataFormat::RGB;
	} else {
		stbi_image_free(data);
		assert(channels && "Format not supported");
	}

	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexImage2D(GL_TEXTURE_2D, 0, DataFormatToGLType(mInternalFormat), mWidth, mHeight, 0,
		DataFormatToGLType(mDataFormat), GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// TODO: Do I need this?
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, DataFormatToGLType(mDataFormat), GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

GLTexture::~GLTexture() {
	glDeleteTextures(1, &mId);
}

void GLTexture::Bind(uint8_t slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mId);
}

void GLTexture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool GLTexture::operator==(const GLTexture& texture) const {
	if (this == &texture)
		return true;

	return mId == texture.mId;
}

std::shared_ptr<GLTexture> GLTexture::Load(const std::filesystem::path& filepath) {
	return std::make_shared<GLTexture>(filepath);
}
