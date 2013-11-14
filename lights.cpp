#include "lights.h"

DirectionalLight::DirectionalLight(Vector3f amb, Vector3f col, Vector3f dir)
{
	ambient = amb;
	color = col;
	direction = dir;
	position = {INFINITY, INFINITY, INFINITY};
}

PointLight::PointLight(Vector3f amb, Vector3f col, Vector3f pos)
{
	ambient = amb;
	color = col;
	position = pos;
}

void DirectionalLight::generateLightRay(LocalGeo& local, Ray& r, Vector3f& col)
{
	r.origin = local.position;
	r.direction = direction;
	r.t_max = INFINITY;
	color = col;
}

void PointLight::generateLightRay(LocalGeo& local, Ray& r, Vector3f& col)
{
	r.origin = local.position;
	r.direction = position - local.position;
	color = col;
}