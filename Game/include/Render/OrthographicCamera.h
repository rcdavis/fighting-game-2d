#pragma once

#include "glm/glm.hpp"

class OrthographicCamera {
public:
	OrthographicCamera(float left, float right, float bottom, float top);

	void SetProjection(float left, float right, float bottom, float top);

	const glm::vec3& GetPosition() const { return mPosition; }
	void SetPosition(const glm::vec3& pos) { mPosition = pos; RecalculateViewMatrix(); }

	const glm::mat4& GetView() const { return mView; }
	const glm::mat4& GetProjection() const { return mProjection; }
	const glm::mat4& GetViewProjection() const { return mViewProjection; }

private:
	void RecalculateViewMatrix();

private:
	glm::mat4 mView;
	glm::mat4 mProjection;
	glm::mat4 mViewProjection;

	glm::vec3 mPosition;
};
