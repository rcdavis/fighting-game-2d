#include "Render/OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
	mView(1.0f),
	mProjection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
	mViewProjection(1.0f),
	mPosition(0.0f)
{
	mViewProjection = mProjection;
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
	mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	mViewProjection = mProjection * mView;
}

void OrthographicCamera::RecalculateViewMatrix() {
	mView = glm::translate(glm::mat4(1.0f), -mPosition);
	mViewProjection = mProjection * mView;
}
