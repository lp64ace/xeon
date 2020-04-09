#pragma once
#include <math/hitable.h>
#include <vector>

/* Holds all the shapes in the scene. */
class ShapeList {
public:
	ShapeList() = default;
	ShapeList(std::vector<Hitable*>& objs) :
		object_list(objs) {}
	~ShapeList();

	/* Add object to the scene. */
	void add_object(Hitable* obj);

	/* Casts a ray and returns if it hit any object. */
	bool cast_ray(const Ray& r, float t_min, float t_max, HitDesc& d);

public:
	std::vector<Hitable*> object_list;
};