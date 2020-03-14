/************************************
* Singleton class that contains the camera
* I decided to do it as a singleton, as we only need
* global camera that looks the entire scene.
************************************/

#include "Camera.h"
#include <string>

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

Camera * Camera::getInstance()
{
	static Camera*  _instance;
	if (!_instance)
	{
		_instance = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	}

	return _instance;
}

void Camera::KeyControl(bool * keys, GLfloat deltaTime, float catAngle)
{
	GLfloat velocity = moveSpeed * deltaTime;
	glm::vec3 newPos = position;


	if (keys[GLFW_KEY_W])
	{
		newPos += frontFixedY * velocity;
		if (!CollisionDetection::AnyObject(newPos, catAngle))
			position = newPos;
	}

	if (keys[GLFW_KEY_S])
	{
		newPos -= frontFixedY * velocity;
		if (!CollisionDetection::AnyObject(newPos, catAngle))
			position = newPos;
	}

	if (keys[GLFW_KEY_A])
	{
		newPos -= rightFixedY * velocity;
		if (!CollisionDetection::AnyObject(newPos, catAngle))
			position = newPos;
	}

	if (keys[GLFW_KEY_D])
	{
		newPos += rightFixedY * velocity;
		if (!CollisionDetection::AnyObject(newPos, catAngle))
			position = newPos;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	Update();
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

glm::mat4 Camera::CalculateViewMatrix()
{
	// if you want to follow a character, change the second argument to focus on character
	return glm::lookAt(position, position + front, up);
}


Camera::~Camera()
{
}

void Camera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	frontFixedY.x = front.x;
	frontFixedY.z = front.z;

	right = glm::normalize(glm::cross(front, worldUp));
	rightFixedY = glm::normalize(glm::cross(frontFixedY, worldUp));
	up = glm::normalize(glm::cross(right, front));

	position.x = glm::clamp(position.x, -MAX_BOUND_X, MAX_BOUND_X);
	position.z = glm::clamp(position.z, -MAX_BOUND_Z, MAX_BOUND_Z);
}
