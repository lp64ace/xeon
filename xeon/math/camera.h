#pragma once
#include "ray.h"

class Camera {
public:
	Camera() = default;
	Camera(const glm::vec3& _origin, const glm::vec3& _lower_left);
	~Camera() = default;

	Ray get_ray(float u, float v);

public:
	glm::vec3 origin, lower_left;
	glm::vec3 horizontal, vertical;
};