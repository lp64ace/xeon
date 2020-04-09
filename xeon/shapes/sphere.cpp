#include "sphere.h"

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitDesc& desc) const
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
			return true;
		}

		temp = (-b + glm::sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			desc.t = temp;
			desc.p = r.point_at(temp);
			desc.normal = (desc.p - center) / radius;
			return true;
		}
	}
	return false;
}
