#pragma once
#include <cuda_runtime.h>
#include <glm/glm.hpp>

class Ray {
public:
	__device__ Ray() = default;
	__device__ Ray(const glm::vec3& _origin, const glm::vec3& _direction) :
		origin(_origin), direction(_direction) {};
	__device__ ~Ray() = default;

	__device__ glm::vec3 Ray::point_at(float t) const
	{
		return origin + t * direction;
	}


public:
	glm::vec3 origin, direction;
};