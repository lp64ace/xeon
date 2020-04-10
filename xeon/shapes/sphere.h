#pragma once
#include <math/hitable.h>

class Material;
class Sphere : public Hitable {
public:
	__device__ Sphere(const glm::vec3& origin, float r, Material* mat) :
		center(origin), radius(r), surface_material(mat) {}
	__device__ ~Sphere() = default;

	__device__ bool Sphere::hit(const Ray& r, float t_min, float t_max, HitDesc& desc) const override
	{
		glm::vec3 oc = r.origin - center;
		auto a = glm::dot(r.direction, r.direction);
		auto b = glm::dot(oc, r.direction);
		auto c = glm::dot(oc, oc) - radius * radius;

		float d = b * b - a * c;
		if (d > 0.0f) {
			float temp = (-b - glm::sqrt(b * b - a * c)) / a;
			if (temp < t_max && temp > t_min) {
				desc.t = temp;
				desc.p = r.point_at(temp);
				desc.normal = (desc.p - center) / radius;
				desc.mat = surface_material;
				return true;
			}

			temp = (-b + glm::sqrt(b * b - a * c)) / a;
			if (temp < t_max && temp > t_min) {
				desc.t = temp;
				desc.p = r.point_at(temp);
				desc.normal = (desc.p - center) / radius;
				desc.mat = surface_material;
				return true;
			}
		}
		return false;
	}

public:
	Material* surface_material;
	glm::vec3 center;
	float radius = 0;
};