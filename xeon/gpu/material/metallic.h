#pragma once
#include <gpu/material.h>

class Metallic : public Material {
public:
	__device__ Metallic(glm::vec3 color) :
		albedo(color) {}
	__device__ ~Metallic() = default;

	__device__ bool Metallic::scatter(const Ray& r, HitDesc& desc, glm::vec3& attenuation, Ray& scattered, curandState* local_rand_state) override
	{
		/* NOTE: Η κατεύθυνση δεν είναι κανονικοποιημένη άρα πρέπει να καλείται glm::normalize */
		/* όποτε επιθυμείται κανονικοποίηση. */
		glm::vec3 reflected = glm::reflect(glm::normalize(r.direction), desc.normal);
		scattered = Ray(desc.p, reflected);
		attenuation = albedo;

		return (glm::dot(scattered.direction, desc.normal) > 0);
	}

public:
	glm::vec3 albedo;
};