#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "core.h"
#include "scene.h"
#include <iostream>

class RayTracer
{
public:
	int threshold;
	Scene* scene;
	Camera* camera;
	RayTracer();
	RayTracer(int t, Scene* s, Camera* c);
	void trace(Ray& ray, int depth, Vector3f* color);
};

#endif