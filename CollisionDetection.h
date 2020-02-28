#pragma once

#include <stdio.h>
#include <glm/glm.hpp>

namespace CollisionDetection
{
	bool Cat(glm::vec3 pos, float catAngle);
	bool Pyramid1(glm::vec3 pos);
	bool Pyramid2(glm::vec3 pos);
}