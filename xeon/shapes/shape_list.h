#pragma once
#include <math/hitable.h>

/* Holds all the shapes in the scene. */
class ShapeList {
public:
	__device__ ShapeList() = default;
	__device__ ShapeList(Hitable** objs, int count) :
		object_list(objs), object_count(count) {}
	__device__ ~ShapeList() = default;

	/* Casts a ray and returns if it hit any object. */
	__device__ bool cast_ray(const Ray& r, float t_min, float t_max, HitDesc& d)
	{
		HitDesc temp_d;
		bool hit_object = false;
		double closest_object_hit = t_max;

		for (int i = 0; i < object_count; i++) {
			Hitable* object = object_list[i];

			if (object->hit(r, t_min, closest_object_hit, temp_d)) {
				hit_object = true;
				closest_object_hit = temp_d.t;
				d = temp_d;
			}
		}

		return hit_object;
	}

public:
	Hitable** object_list;
	int object_count = 0;
};