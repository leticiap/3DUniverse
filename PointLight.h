#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "OmniShadowMap.h"
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
			   GLfloat near, GLfloat far,
			   GLfloat red, GLfloat green, GLfloat blue, 
			   GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColourLocation,
				  unsigned int diffuseIntensityLocation, unsigned int positionLocation,
				  unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

	~PointLight();

protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;

	GLfloat farPlane;
};

