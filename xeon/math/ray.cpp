#include "ray.h"

glm::vec3 Ray::point_at(float t) const
{
	return origin + t * direction;
}
