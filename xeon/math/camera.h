#pragma once
#include "ray.h"

class Camera {
public:
	__device__ Camera() = default;
	__device__ Camera(const glm::vec3& _origin, const glm::vec3& _lower_left) :
		origin(_origin), lower_left(_lower_left)
	{
		int width = abs(2 * lower_left.x);
		horizontal = glm::vec3(width, 0, 0);

		int height = abs(2 * lower_left.y);
		vertical = glm::vec3(0, height, 0);
	}
	__device__ ~Camera() = default;

	__device__ Ray Camera::get_ray(float u, float v)
	{
		return Ray(origin, lower_left + u * horizontal + v * vertical);
	}

public:
	glm::vec3 origin, lower_left;
	glm::vec3 horizontal, vertical;
};