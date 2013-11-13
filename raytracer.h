#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "scene.h"
#include <iostream>

class RayTracer
{
public:
	int threshold;
	Scene* scene;
	RayTracer();
	RayTracer(int t, Scene* s);
	void trace(Ray& ray, int depth, Vector3f* color);
};

#endif