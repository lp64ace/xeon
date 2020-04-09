#pragma once
#include "ray.h"
#include "camera.h"
#include "random.h"
#include <shapes/shape_list.h>

glm::vec3 color(const Ray& r, ShapeList& world);
glm::vec3 get_random_sphere_unit_point();

double squared_length(const glm::vec3& v);