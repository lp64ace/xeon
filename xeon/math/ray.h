#pragma once
#include <glm/glm.hpp>

class Ray {
public:
	Ray() = default;
	Ray(const glm::vec3& _origin, const glm::vec3& _direction) :
		origin(_origin), direction(_direction) {};
	~Ray() = default;

	glm::vec3 point_at(float t) const;

public:
	glm::vec3 origin, direction;
};