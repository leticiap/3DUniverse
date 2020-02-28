#include "CollisionDetection.h"

namespace CollisionDetection
{
	bool Cat(glm::vec3 pos, float catAngle)
	{
		glm::vec3 catPos = glm::vec3(0.0f, 0.0f, 0.0f);
		catPos.x = cos(glm::radians(catAngle)) * 5.0f;
		catPos.z = sin(glm::radians(catAngle)) * 5.0f;
		float catRad = 3.0f;
		if (pos.x < catPos.x + catRad && pos.x > catPos.x - catRad)
		{
			if (pos.z < catPos.z + catRad && pos.z > catPos.z - catRad)
				return true;
		}
		return false;
	}
	bool Pyramid1(glm::vec3 pos)
	{
		glm::vec3 pyramidPos = glm::vec3(0.0f, -1.5f, -7.5f);
		float pyramidRad = 1.5f;
		if (pos.x < pyramidPos.x + pyramidRad && pos.x > pyramidPos.x - pyramidRad)
		{
			if (pos.z < pyramidPos.z + pyramidRad && pos.z > pyramidPos.z - pyramidRad)
				return true;
		}
		return false;
	}
	bool Pyramid2(glm::vec3 pos)
	{
		glm::vec3 pyramidPos = glm::vec3(0.0f, -1.5f, -7.5f);
		float pyramidRad = 1.5f;
		if (pos.x < pyramidPos.x + pyramidRad && pos.x > pyramidPos.x - pyramidRad)
		{
			if (pos.z < pyramidPos.z + pyramidRad && pos.z > pyramidPos.z - pyramidRad)
				return true;
		}
		return false;
	}
}