#include "math.h"
#include <climits>
#include <glm/gtx/compatibility.hpp>

glm::vec3 color(const Ray& r, ShapeList& world,int maxbounces) {
	HitDesc desc;

	if (maxbounces > 0 && world.cast_ray(r, 0.001f, FLT_MAX, desc)) {
		glm::vec3 reflected = 0.5f * color ( Ray ( desc.p , glm::reflect ( r.direction , desc.normal ) ) , world , maxbounces / 2 ) ;

		glm::vec3 target = desc.p + desc.normal + get_random_sphere_unit_point();
		glm::vec3 rough = 0.5f * color ( Ray ( desc.p , target - desc.p ) , world , maxbounces / 2 ) ;
		return glm::lerp ( rough , reflected , 0.01f ) ;
	}
	else {
		auto unit_dir = glm::normalize(r.direction);
		float t = 0.5 * (unit_dir.y + 1.0);

		return glm::lerp(glm::vec3(1.0f), glm::vec3(0.5, 0.7, 1.0), t);
	}
}

glm::vec3 get_random_sphere_unit_point()
{
	glm::vec3 p;

	do {
		p = 2.0f * glm::vec3(drand48(), drand48(), drand48()) - glm::vec3(1);
	} while (squared_length(p) >= 1.0);

	return p;
}

double squared_length(const glm::vec3& v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}