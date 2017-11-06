#include "Camera.h"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw, float pitch) 
	: m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY)
{
	m_pos = position;
	m_worldUp = up;
	m_yaw = yaw;
	m_pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY)
{
	m_pos = glm::vec3(posX, posY, posZ);
	m_worldUp = glm::vec3(upX, upY, upZ);
	m_yaw = yaw;
	m_pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_movementSpeed*deltaTime;
	if (direction & CameraMovement::eForward)
		m_pos += m_front*velocity;
	if (direction & CameraMovement::eBackward )
		m_pos -= m_front*velocity;
	if (direction & CameraMovement::eLeft)
		m_pos -= m_right*velocity;
	if (direction & CameraMovement::eRight)
		m_pos += m_right*velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainm_pitch) {

	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;
	m_yaw   += xoffset;
	m_pitch += yoffset;
	if (constrainm_pitch) {
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = 89.0f;
	}
	updateCameraVectors();
	return;
}

void Camera::updateCameraVectors() {
	m_front.x = cos(glm::radians(m_yaw))*cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw))*cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::vec3 Camera::getPositon() const
{
	return glm::vec3(m_pos);
}













