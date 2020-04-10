#pragma once
#include <gpu/material.h>
#include <math/math.h>

class Diffuse : public Material {
public:
	__device__ Diffuse(const glm::vec3& color) :
		albedo(color) {}
	__device__ ~Diffuse() = default;

	__device__ bool Diffuse::scatter(const Ray& r, HitDesc& desc, glm::vec3& attenuation, Ray& scattered, curandState* local_rand_state) override
	{
		glm::vec3 target = desc.p + desc.normal + get_random_sphere_unit_point(local_rand_state);
		scattered = Ray(desc.p, target - desc.p);
		attenuation = albedo;
		return true;
	}


public:
	glm::vec3 albedo;
};