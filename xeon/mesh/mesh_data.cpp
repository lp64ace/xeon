#include "mesh_data.h"

#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Face::genFlatNormal ( ) {
	return ( N = glm::normalize ( glm::cross ( vert [ 1 ].pos - vert [ 0 ].pos , vert [ 2 ].pos - vert [ 0 ].pos ) ) ) ;
}
