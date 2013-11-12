#include "lights.h"

DirectionalLight::DirectionalLight(Vector3f amb, Vector3f col, Vector3f dir)
{
	ambient = amb;
	color = col;
	direction = dir;
}

PointLight::PointLight(Vector3f amb, Vector3f col, Vector3f pos)
{
	ambient = amb;
	color = col;
	position = pos;
}