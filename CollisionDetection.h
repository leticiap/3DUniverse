#pragma once

#include <stdio.h>
#include <glm/glm.hpp>

namespace CollisionDetection
{
	bool AnyObject(glm::vec3 pos, float catAngle);
	bool Cat(glm::vec3 pos, float catAngle, float catRad);
	bool Pyramid1(glm::vec3 pos, float pyramidRad);
	bool Pyramid2(glm::vec3 pos, float pyramidRad);
}