#pragma once
#include <gpu/material.h>

class Transparent : public Material {
public:
	__device__ Transparent(float ref_index) :
		refractive_index(ref_index) {};
	__device__ ~Transparent() = default;

	__device__ bool Transparent::scatter(const Ray& r, HitDesc& d, glm::vec3& attenuation, Ray& scattered, curandState* local_rand_state) override
	{
		return false;
	}

public:
	float refractive_index;
};