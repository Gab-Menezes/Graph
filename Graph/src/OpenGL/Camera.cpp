#include <pch.h>
#include "Camera.h"

Camera::Camera(const glm::vec3 & pos, const glm::vec3 & front, const glm::vec3 & up,  float yaw, float pitch, float sens, float speed) :
	m_CamPos(pos), m_CamFront(front), m_CamUp(up),
	m_Yaw(yaw), m_Pitch(pitch), m_Sensitivity(sens),
	m_MoveSpeed(speed), m_InitYPos(m_CamPos.y),
	m_Window(glfwGetCurrentContext())
{
	//Get the pmouse
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	m_PMousePos = { (float)xpos, (float)ypos };
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);
}

//SETTERS
void Camera::SetCamPos(const glm::vec3 & pos)
{
	m_CamPos = pos;
}

void Camera::SetCamFront(const glm::vec3 & front)
{
	m_CamFront = front;
}

void Camera::SetCamUp(const glm::vec3 & up)
{
	m_CamUp = up;
}

void Camera::SetMoveSpeed(float speed)
{
	m_MoveSpeed = speed;
}

void Camera::SetSensitivity(float sens)
{
	m_Sensitivity = sens;
}

//GETTERS
glm::vec3 Camera::GetCamPos() const
{
	return m_CamPos;
}

glm::vec3 Camera::GetCamFront() const
{
	return m_CamFront;
}

std::pair<float, float> Camera::GetCamAngles() const
{
	return std::make_pair(m_Yaw, m_Pitch);
}

//UPDATE
bool Camera::MouseMove()
{
	//Get current mouse pos
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);

	//Offset the mouse pos with the pmouse pos
	float xOff = ((float)xpos - m_PMousePos.first) * m_Sensitivity;
	float yOff = (m_PMousePos.second - (float)ypos) * m_Sensitivity;
	m_PMousePos = { xpos, ypos };//pmouse pos = current mouse pos

	if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (xOff != 0.f || yOff != 0.f))
	{
		//MAGIC NUMBER
		float multiplier = 0.0165f * m_CamPos.z;

		float camSpeedX = (float)xOff * multiplier;
		float camSpeedY = (float)yOff * multiplier;

		m_CamPos -= glm::normalize(glm::cross(m_CamFront, m_CamUp)) * camSpeedX;
		m_CamPos -= camSpeedY * m_CamUp;

		return true;
	}

	return false;
}

void Camera::KeyboardMove(float deltaTime)
{
	float camSpeed = m_MoveSpeed * deltaTime;
	//Update the position based on the key pressed
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_CamPos += camSpeed * m_CamFront;
	else if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_CamPos -= camSpeed * m_CamFront;
	else if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_CamPos -= glm::normalize(glm::cross(m_CamFront, m_CamUp)) * camSpeed;
	else if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_CamPos += glm::normalize(glm::cross(m_CamFront, m_CamUp)) * camSpeed;

	/*if (m_FpsStyle)
		m_CamPos.y = m_InitYPos;*/
}
