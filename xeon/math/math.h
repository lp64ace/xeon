#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../mesh/mesh_data.h"

struct Ray {
	glm::vec3 origin ;
	glm::vec3 dir ;
};

struct Face {
	Vert vert [ 3 ] ;
	glm::vec3 N ;

	glm::vec3 genFlatNormal ( ) ;
};

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
bool rayTriangleIntersect ( const Ray &ray , const Face &v , float &t ) ;
