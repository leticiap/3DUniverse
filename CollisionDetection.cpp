#include "CollisionDetection.h"

namespace CollisionDetection
{
	bool AnyObject(glm::vec3 pos, float catAngle)
	{
		return Pyramid1(pos, 1.5f) || Pyramid2(pos, 1.5f) || Cat(pos, catAngle, 2.0f);
	}

	bool Cat(glm::vec3 pos, float catAngle, float catRad)
	{
		glm::vec3 catPos = glm::vec3(0.0f, 0.0f, 0.0f);
		catPos.x = cos(glm::radians(catAngle)) * 10.0f;
		catPos.z = sin(glm::radians(catAngle)) * 10.0f;
		if (pos.x < catPos.x + catRad && pos.x > catPos.x - catRad)
		{
			if (pos.z < catPos.z + catRad && pos.z > catPos.z - catRad)
				return true;
		}
		return false;
	}
	bool Pyramid1(glm::vec3 pos, float pyramidRad)
	{
		glm::vec3 pyramidPos = glm::vec3(0.0f, -1.5f, -7.5f);
		if (pos.x < pyramidPos.x + pyramidRad && pos.x > pyramidPos.x - pyramidRad)
		{
			if (pos.z < pyramidPos.z + pyramidRad && pos.z > pyramidPos.z - pyramidRad)
				return true;
		}
		return false;
	}
	bool Pyramid2(glm::vec3 pos, float pyramidRad)
	{
		glm::vec3 pyramidPos = glm::vec3(0.0f, -1.5, 3.0f);
		if (pos.x < pyramidPos.x + pyramidRad && pos.x > pyramidPos.x - pyramidRad)
		{
			if (pos.z < pyramidPos.z + pyramidRad && pos.z > pyramidPos.z - pyramidRad)
				return true;
		}
		return false;
	}
}