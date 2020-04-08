#include "Transform.h"

Transform::Transform ( ) {
	_translation = glm::vec3 ( 0.0f , 0.0f , 0.0f ) ;
	_scalation = glm::vec3 ( 1.0f , 1.0f , 1.0f ) ;
	_rotation = glm::vec3 ( 0.0f , 0.0f , 0.0f ) ;

	fov = 90.0f ;
	aspect = 1.0f ;
	near = 0.1f ;
	far = 100.0f ;
	left = -1.0f ;
	right = 1.0f ;
	top = 1.0f ;
	bottom = -1.0f ;

	type = transformation ;
	parent = NULL ;
}

Transform::Transform ( const Transform &other ) {
	_translation = other._translation ;
	_rotation = other._rotation ;
	_scalation = other._scalation ;

	fov = other.fov ;
	aspect = other.aspect ;
	near = other.near ;
	far = other.far ;
	left = other.left ;
	right = other.right ;
	top = other.top ;
	bottom = other.bottom ;

	type = other.type ;

	parent = other.parent ;
}

Transform::Transform ( const glm::vec3 &position , const glm::vec3 &rotation , const glm::vec3 &scalation ) {
	_translation = position ;
	_rotation = rotation ;
	_scalation = scalation ;
	
	type = transformation ;
	parent = NULL ;
}

Transform::Transform ( const glm::vec3 &position , const glm::quat &rotation , const glm::vec3 &scalation ) {
	_translation = position ;
	_rotation = glm::eulerAngles ( rotation ) ;
	_scalation = scalation ;

	type = transformation ;
	parent = NULL ;
}

Transform::Transform ( float fov , float aspect , float near , float far ) {
	this->fov = fov ;
	this->aspect = aspect ;
	this->near = near ;
	this->far = far ;

	this->_translation = glm::vec3 ( 0.0f ) ;
	this->_scalation = glm::vec3 ( 1.0f ) ;
	this->_rotation = glm::vec3 ( 0.0f ) ;

	type = perspective ;
	parent = NULL ;
}

Transform::Transform ( float left , float right , float top , float bottom , float near , float far ) {
	this->near = near ;
	this->far = far ;
	this->left = left ;
	this->right = right ;
	this->top = top ;
	this->bottom = bottom ;

	this->_translation = glm::vec3 ( 0.0f ) ;
	this->_rotation = glm::vec3 ( 0.0f ) ;
	this->_scalation = glm::vec3 ( 1.0f ) ;

	type = orthographic ;
	parent = NULL ;
}

Transform::~Transform ( ) {

}

glm::vec3 Transform::get_translation ( ) const {
	return _translation ;
}

glm::vec3 Transform::get_rotation_xyz ( ) const {
	return _rotation ;
}

glm::quat Transform::get_rotation_quat ( ) const {
	return glm::toQuat ( glm::orientate3 ( _rotation ) ) ;
}
glm::vec3 Transform::get_scalion ( ) const {
	return _scalation ;
}

float Transform::get_aspect ( ) const {
	return aspect ;
}

float Transform::get_fov ( ) const {
	return fov ;
}

Transform &Transform::set_translation ( glm::vec3 position ) {
	_translation = position ;
	return *this ;
}
Transform &Transform::set_rotation_xyz ( glm::vec3 euler_angles ) {
	_rotation = glm::radians ( euler_angles ) ;
	return *this ;
}
Transform &Transform::set_rotation_quat ( glm::quat quaternion ) {
	_rotation = glm::eulerAngles ( quaternion ) ;
	return *this ;
}
Transform &Transform::set_scalation ( glm::vec3 scale ) {
	_scalation = scale ;
	return *this ;
}

void Transform::set_aspect ( float aspect ) {
	this->aspect = aspect ;
}

void Transform::set_fov ( float fov ) {
	this->fov = fov ;
}

void Transform::set_type ( Transform::Type type ) {
	this->type = type ;
}

Transform &Transform::translate ( glm::vec3 offset ) {
	_translation += offset ;
	return *this ;
}
Transform &Transform::rotate ( float angle , glm::vec3 axis ) {
	_rotation += glm::radians ( angle ) * axis ;
	return *this ;
}
Transform &Transform::scale ( glm::vec3 scale ) {
	_scalation *= scale ;
	return *this ;
}

Transform &Transform::link ( Transform *parent ) {
	this->parent = parent ;
	return *this ;
}

glm::vec3 Transform::calc_front ( ) const {
	return glm::vec3 ( cos ( _rotation.x ) * sin ( _rotation.y ) ,
		sin ( _rotation.x ) , cos ( _rotation.x ) * cos ( _rotation.y ) );
}

glm::vec3 Transform::calc_right ( ) const {
	return glm::vec3 ( sin ( _rotation.y - glm::half_pi<float> ( ) ) , 0 , cos ( _rotation.y - glm::half_pi<float> ( ) ) );
}

glm::vec3 Transform::calc_up ( ) const {
	return glm::cross ( calc_right ( ) , calc_front ( ) ) ;
}

glm::mat4 Transform::tranformation_matrix ( ) const {
	if ( type != Transform::Type::transformation ) return glm::mat4 ( 1.0 ) ;
	return glm::mat4 ( glm::translate ( _translation ) * glm::eulerAngleYXZ ( _rotation.y , _rotation.x , _rotation.z ) * glm::scale ( _scalation ) ) ;
}

glm::mat4 Transform::view_matrix ( ) const {
	glm::mat4 view = glm::mat4 ( 1.0f ) ;

	if ( type == Transform::Type::perspective ) {
		view = glm::perspective ( glm::radians ( fov ) , aspect , near , far ) ;
	}
	if ( type == Transform::Type::orthographic ) {
		view = glm::ortho ( left , right , bottom , top , near , far ) ;
	}
	if ( type != Transform::Type::transformation ) {
		glm::vec3 r = _rotation ;
		view *= glm::lookAt ( _translation , _translation + calc_front ( ) , glm::cross ( calc_right ( ) , calc_front ( ) ) ) ;
	}

	return view ;
}

glm::mat4 Transform::to_matrix ( ) const {
	glm::mat4 par = ( parent ) ? parent->to_matrix ( ) : glm::mat4 ( 1.0f ) ;
	return par * ( view_matrix ( ) * tranformation_matrix ( ) ) ;
}

Transform &Transform::operator = ( const Transform &transform ) {
	_translation = transform._translation ;
	_rotation = transform._rotation ;
	_scalation = transform._scalation ;

	fov = transform.fov ;
	aspect = transform.aspect ;
	near = transform.near ;
	far = transform.far ;
	left = transform.left ;
	right = transform.right ;
	top = transform.top ;
	bottom = transform.bottom ;

	type = transform.type ;

	parent = transform.parent ;

	return *this ;
}
