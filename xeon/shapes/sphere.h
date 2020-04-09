#pragma once
#include <math/hitable.h>

class Sphere : public Hitable {
public:
	Sphere(const glm::vec3& origin, float r) :
		center(origin), radius(r) {}
	~Sphere() = default;

	bool hit(const Ray& r, float t_min, float t_max, HitDesc& d) const override;

public:
	glm::vec3 center;
	float radius = 0;
};