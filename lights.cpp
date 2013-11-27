#include "lights.h"

DirectionalLight::DirectionalLight(Vector3f i, Vector3f col, Vector3f dir)
{
	intensity = i;
	color = col;
	direction = dir;
	direction.normalize();
	position = {INFINITY, INFINITY, INFINITY};
}

PointLight::PointLight(Vector3f i, Vector3f col, Vector3f pos)
{
	intensity = i;
	color = col;
	position = pos;
}

void DirectionalLight::generateLightRay(LocalGeo& local, Ray& r, Vector3f& col)
{
	r.direction = -direction;
	/* Displace ray by small amount so that it doesn't intersect with the local primitive */
	r.origin = local.position + 0.0001 * r.direction ;
	r.t_max = INFINITY;
	color = col;
}

void PointLight::generateLightRay(LocalGeo& local, Ray& r, Vector3f& col)
{
	r.direction = position - local.position;
	r.direction.normalize();
	/* Displace ray by small amount so that it doesn't intersect with the local primitive */
	r.origin = local.position + 0.0001 * r.direction;
	color = col;
}