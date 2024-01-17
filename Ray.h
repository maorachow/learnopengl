#pragma once
#include <glm\glm.hpp>
#include <glm/geometric.hpp>

class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	Ray(glm::vec3 origin,glm::vec3 dir) {
		this->origin = origin;
		this->direction = glm::normalize(dir);
	}
};
static glm::vec3 vec3Lerp(glm::vec3 v1, glm::vec3 v2, float t) {
	float f1 = std::lerp(v1.x, v2.x, t);
	float f2 = std::lerp(v1.y, v2.y, t);
	float f3 = std::lerp(v1.z, v2.z, t);
	glm::vec3 value = glm::vec3(f1, f2, f3);
	return value;
}

