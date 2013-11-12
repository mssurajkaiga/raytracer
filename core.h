#ifndef CORE_H
#define CORE_H

#include <Eigen/Dense>
using namespace Eigen;

/* A ray */
struct Ray
{
	Vector3f origin;
	Vector3f direction;
	float t_min, t_max; 
	/* ray(t) = origin + t*direction, where t_min <= t <= t_max */
};

struct Brdf
{
	Vector3f kd, ks, ka, kr;
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