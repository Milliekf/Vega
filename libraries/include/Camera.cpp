#include "Camera.h"

CCamera::CCamera(glm::vec3 vPosition, glm::vec3 vUp, float vYaw, float vPitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(0.2f), m_MouseSensitivity(0.02f), m_Zoom(45.0f)
{
	m_Position = vPosition;
	m_WorldUp = vUp;
	m_Yaw = vYaw;
	m_Pitch = vPitch;
	__updateCameraVectors();
}

CCamera::CCamera(float vPosX, float vPosY, float vPosZ, float vUpX, float vUpY, float vUpZ, float vYaw, float vPitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(0.2f), m_MouseSensitivity(0.02f), m_Zoom(45.0f)
{
	m_Position = glm::vec3(vPosX, vPosY, vPosZ);
	m_WorldUp = glm::vec3(vUpX, vUpY, vUpZ);
	m_Yaw = vYaw;
	m_Pitch = vPitch;
	__updateCameraVectors();
}

//****************************************************************************************************
//FUNCTION:
void CCamera::processKeyboard(ECamera_Movement vDirection, float vDeltaTime)
{
	const float Velocity = m_MovementSpeed * vDeltaTime * 20;

	if (vDirection == FORWARD)
		m_Position += m_Front * Velocity;
	if (vDirection == BACKWARD)
		m_Position -= m_Front * Velocity;
	if (vDirection == LEFT)
		m_Position -= m_Right * Velocity;
	if (vDirection == RIGHT)
		m_Position += m_Right * Velocity;
}

//****************************************************************************************************
//FUNCTION:
void CCamera::processMouseMovement(float vXOffset, float vYOffset, GLboolean vConstrainPitch)
{
	vXOffset *= m_MouseSensitivity;
	vYOffset *= m_MouseSensitivity;

	m_Yaw += vXOffset;
	m_Pitch += vYOffset;

	if (vConstrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}
	__updateCameraVectors();
}

//****************************************************************************************************
//FUNCTION:
void CCamera::processMouseScroll(float vYOffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= vYOffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}

//****************************************************************************************************
//FUNCTION:
void CCamera::__updateCameraVectors()
{
	glm::vec3 Front;
	Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	Front.y = sin(glm::radians(m_Pitch));
	Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(Front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}