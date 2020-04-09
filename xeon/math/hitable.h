#pragma once
#include "ray.h"

struct HitDesc {
	float t;
	glm::vec3 p, normal;
};

class Hitable {
public:
	Hitable() = default;
	~Hitable() = default;

	virtual bool hit(const Ray& r, float t_min, float t_max, HitDesc& d) const = 0;
};