#pragma once
#include "ray.h"
#include "camera.h"
#include <gpu/material.h>
#include <glm/gtx/compatibility.hpp>
#include <shapes/shape_list.h>

#define RANDVEC3 glm::vec3(curand_uniform(local_rand_state),curand_uniform(local_rand_state),curand_uniform(local_rand_state))

__device__ double squared_length(const glm::vec3& v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

__device__ glm::vec3 advance_ray(const Ray& r, ShapeList* world, int maxbounces, curandState* local_rand_state) {
    HitDesc desc;
    if (maxbounces > 0 && world->cast_ray(r, 0.001f, FLT_MAX, desc)) {
        Ray scattered;
        glm::vec3 attenuation;

        if (desc.mat->scatter(r, desc, attenuation, scattered, local_rand_state))
            return attenuation * advance_ray(scattered, world, maxbounces - 1, local_rand_state);
        else
            return glm::vec3(0.0f);
    }
    else {
        auto unit_dir = glm::normalize(r.direction);
        float t = 0.5 * (unit_dir.y + 1.0);

        return glm::lerp(glm::vec3(1.0f), glm::vec3(0.5, 0.7, 1.0), t);
    }
}

__device__ glm::vec3 get_random_sphere_unit_point(curandState* local_rand_state)
{
	glm::vec3 p;

	do {
		p = 2.0f * RANDVEC3 - glm::vec3(1);
	} while (squared_length(p) >= 1.0f);

	return p;
}