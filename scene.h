#ifndef SCENE_H
#define SCENE_H

#include <Eigen/Dense>
#include "core.h"
#include "lights.h"
#include "objects.h"
using namespace Eigen;

class Sampler
{
public:
	float x_max, y_max;
	Sampler();
	Sampler(float x, float y);
	bool getSample(Sample* sample);
};

class Camera	/* Currently not used as camera is assumed to be at origin */
{
public:
	Vector3f position, lookAt, up;
	Vector3f u, v, w; /* coordinate system generated from camera and viewing plane positions */
	float fov_x, fov_y; /* field of view */
	int width, height; /* Image plane dimensions */
	Camera(Vector3f pos, Vector3f lAt, Vector3f u_, float fx, float fy, int wid, int hei);
	void generateRay(Sample& sample, Ray* ray);
};
/*
class Scene
{
public:
	Scene(Object* objects, Light* lights);

};
*/

#endif