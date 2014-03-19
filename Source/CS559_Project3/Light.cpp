#include "Light.h"

Light::Light(
	vec3 La /* = DEFAULT_LA */, 
	vec3 Ld /* = DEFAULT_LD */, 
	vec3 Ls /* = DEFAULT_LS */, 
	vec3 lp /* = DEFAULT_LIGHT_POSITION */
):La_(La), Ld_(Ld), Ls_(Ls), Light_Position_(lp)
{

}

Light::~Light() {

}

const GLfloat * Light::getLaValuePtr()const {
	return value_ptr(La_);
}

const GLfloat * Light::getLdValuePtr()const {
	return value_ptr(Ld_);
}

const GLfloat * Light::getLsValuePtr()const {
	return value_ptr(Ls_);
}

const GLfloat * Light::getLightPositionValuePtr()const
{
	return value_ptr(Light_Position_);
}