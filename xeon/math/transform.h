#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#undef near
#undef far

class Transform {
public:	
	enum Type {
		transformation ,
		perspective ,
		orthographic
	};
private:	
	Transform *parent ;
	Type type ;

protected:
	glm::vec3 _rotation ;
	glm::vec3 _scalation ;
	glm::vec3 _translation ;

	float fov ;
	float aspect ;
	float near ;
	float far ;
	float left ;
	float right ;
	float top ;
	float bottom ;
public:
	Transform ( ) ;
	Transform ( const Transform & other ) ;
	Transform ( const glm::vec3 &position , const glm::vec3 &rotation = glm::vec3 ( 0.0f ) , const glm::vec3 &scalation = glm::vec3 ( 1.0f ) ) ;
	Transform ( const glm::vec3 &position , const glm::quat &rotation = glm::quat ( 1.0f , 0.0f , 0.0f , 0.0f ) , const glm::vec3 &scalation = glm::vec3 ( 1.0f ) ) ;

	Transform ( float fov , float aspect , float near , float far ) ;
	Transform ( float left , float right , float top , float bottom , float near , float far ) ;
	~Transform ( ) ;

	glm::vec3 get_translation ( ) const ;
	glm::vec3 get_rotation_xyz ( ) const ;
	glm::quat get_rotation_quat ( ) const ;
	glm::vec3 get_scalion ( ) const ;

	float get_aspect ( ) const ;
	float get_fov ( ) const ;

	Transform &set_translation ( glm::vec3 ) ;
	Transform &set_rotation_xyz ( glm::vec3 ) ;
	Transform &set_rotation_quat ( glm::quat ) ;
	Transform &set_scalation ( glm::vec3 ) ;

	void set_aspect ( float ) ;
	void set_fov ( float ) ;

	void set_type ( Transform::Type type ) ;

	Transform &translate ( glm::vec3 ) ;
	Transform &rotate ( float , glm::vec3 ) ;
	Transform &scale ( glm::vec3 ) ;

	glm::vec3 calc_front ( ) const ;
	glm::vec3 calc_right ( ) const ;
	glm::vec3 calc_up ( ) const ;

	Transform &link ( Transform *parent ) ;

	inline void clear_view ( ) { type = transformation ; }

	glm::mat4 to_matrix ( ) const ;
	glm::mat4 tranformation_matrix ( ) const ;
	glm::mat4 view_matrix ( ) const ;

	Transform &operator = ( const Transform &transform ) ;
};

