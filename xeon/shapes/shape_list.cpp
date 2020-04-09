#include "shape_list.h"

ShapeList::~ShapeList()
{
	for (auto& obj : object_list) {
		delete obj;
	}
}

void ShapeList::add_object(Hitable* obj)
{
	object_list.push_back(obj);
}

bool ShapeList::cast_ray(const Ray& r, float t_min, float t_max, HitDesc& d)
{
	HitDesc temp_d;
	bool hit_object = false;
	double closest_object_hit = t_max;

	for (auto& object : object_list) {
		if (object->hit(r, t_min, closest_object_hit, temp_d)) {
			hit_object = true;
			closest_object_hit = temp_d.t;
			d = temp_d;
		}
	}

	return hit_object;
}
