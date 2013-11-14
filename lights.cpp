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
	r.direction = direction;
	/* Displace ray by small amount so that it doesn't intersect with the local primitive */
	r.origin = local.position + 0.0001 * r.direction ;
	r.t_max = INFINITY;
	color = col;
}

void PointLight::generateLightRay(LocalGeo& local, Ray& r, Vector3f& col)
{
	r.direction = position - local.position;
	r.origin = local.position + 0.0001 * r.direction;
	color = col;
}