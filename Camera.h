#pragma once
#ifndef CAMERA_H_
#define CAMERA_H_
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 25.0f;
constexpr float SENSITIVTY = 0.25f;
constexpr float ZOOM = 0.45f;

enum class CameraMovement {
	eStable=0, eForward=1, eBackward=2, eLeft=4, eRight=8
};

static inline CameraMovement operator |(const CameraMovement &lhs, const CameraMovement &rhs) 
{
	return static_cast<CameraMovement>(static_cast<int>(lhs)|static_cast<int>(rhs));
}

static inline int operator &(const CameraMovement &lhs, const CameraMovement &rhs) 
{
	return static_cast<int>(lhs)&static_cast<int>(rhs);
}

class Camera {
	public:
		Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f), 
			float yaw = YAW, float pitch = PITCH);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		glm::mat4 getViewMatrix();
		void processKeyboard(CameraMovement direction, float deltaTime);
		void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void processMouseScroll(float yoffset);
		glm::vec3 getPositon() const;

	private:
		glm::vec3 m_pos;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;
		float m_yaw;
		float m_pitch;
		float m_movementSpeed;
		float m_mouseSensitivity;
		void updateCameraVectors();
};


#endif