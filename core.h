#ifndef CORE_H
#define CORE_H

#include <math.h>
#include <Eigen/Dense>
using namespace Eigen;

#define pi 3.14159

/* A ray */
struct Ray
{
	Vector3f origin;
	Vector3f direction;
	float t_min, t_max;
	Ray()
	{
		origin = Vector3f(0,0,0);
		direction = Vector3f(0,0,0);
		t_min = 0;
		t_max = INFINITY;
	}
	/* ray(t) = origin + t*direction, where t_min <= t <= t_max */
};

struct Brdf
{
	Vector3f kd, ks, ka, kr;
	float s;
};

struct Sample
{
	float x,y; /* Screen coordinate */
};

struct LocalGeo
{
	Vector3f position, normal;
	/* May need to store textures as well later */
};

#endif