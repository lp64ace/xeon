#include "math.h"

bool rayTriangleIntersect ( const Ray &ray , const Face &f , float &t ) {

	// Check if the ray and the triangle are pararell

	float NdotRayDirection = glm::dot ( f.N , ray.dir ) ;
	if ( fabs ( NdotRayDirection ) < 1e-7 )
		return false ;

	// Check if ray is on the oposite side of the triangle

	float d = glm::dot ( f.N , f.vert [ 0 ].pos ) ;

	t = ( glm::dot ( f.N , ray.origin ) + d ) / NdotRayDirection ;
	if ( t < 0 ) return false;

	// Compute the intersection point
	glm::vec3 P = ray.origin + t * ray.dir;
	glm::vec3 C ;

	glm::vec3 edge0 = f.vert [ 1 ].pos - f.vert [ 0 ].pos ;
	glm::vec3 vp0 = P - f.vert [ 0 ].pos ;
	C = glm::cross ( edge0 , vp0 ) ;
	if ( glm::dot ( f.N , C ) < 0 ) return false;

	glm::vec3 edge1 = f.vert [ 2 ].pos - f.vert [ 1 ].pos ;
	glm::vec3 vp1 = P - f.vert [ 1 ].pos ;
	C = glm::cross ( edge1 , vp1 ) ;
	if ( glm::dot ( f.N , C ) < 0 ) return false;

	glm::vec3 edge2 = f.vert [ 0 ].pos - f.vert [ 2 ].pos ;
	glm::vec3 vp2 = P - f.vert [ 2 ].pos ;
	C = glm::cross ( edge2 , vp2 ) ;
	if ( glm::dot ( f.N , C ) < 0 ) return false;

	return true;
}
