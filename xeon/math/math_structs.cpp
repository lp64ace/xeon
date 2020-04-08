#include "math.h"

glm::vec3 Face::genFlatNormal ( ) {
	return ( N = glm::normalize ( glm::cross ( vert [ 1 ].pos - vert [ 0 ].pos , vert [ 2 ].pos - vert [ 0 ].pos ) ) ) ;
}
