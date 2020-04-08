#pragma once

#include <glm/glm.hpp>

struct Vert {
	glm::vec3 pos ;
};

struct Face {
	Vert vert [ 3 ] ;
	glm::vec3 N ;

	glm::vec3 genFlatNormal ( ) ;
};

