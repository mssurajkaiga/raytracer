#ifndef LIGHTS_H
#define LIGHTS_H
#include "core.h"
#include <Eigen/Dense>
using namespace Eigen;

/* Abstract Light source */
class Light
{
public:
	Vector3f position, intensity, color;
	float attenuation; /* Attenuation factor. If 0, no attenuation. Max value of 1. */
	virtual void generateLightRay(LocalGeo& local, Ray& ray, Vector3f& col) = 0; /* Generate ray from a position to light source */
};

class DirectionalLight: public Light
{
public:
	Vector3f direction;
	DirectionalLight(Vector3f i, Vector3f col, Vector3f dir);
	void generateLightRay(LocalGeo& local, Ray& ray, Vector3f& col);
};

class PointLight: public Light
{
public:
	PointLight(Vector3f i, Vector3f col, Vector3f pos);
	void generateLightRay(LocalGeo& local, Ray& r, Vector3f& col);
};

#endif