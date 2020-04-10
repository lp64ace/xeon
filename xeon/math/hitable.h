#pragma once
#include "ray.h"

class Material;

struct HitDesc {
	float t;
	glm::vec3 p, normal;
	Material* mat;
};

class Hitable {
public:
	__device__ Hitable() = default;
	__device__ ~Hitable() = default;

	__device__ virtual bool hit(const Ray& r, float t_min, float t_max, HitDesc& d) const = 0;
};