#pragma once
#include <math/hitable.h>
#include <curand_kernel.h>

class Material {
public:
	Material() = default;
	~Material() = default;

	__device__ virtual bool scatter(const Ray& r, HitDesc& d, glm::vec3& attenuation, Ray& scattered, curandState* local_rand_state) = 0;
};