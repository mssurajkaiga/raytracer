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
	Vector3f blinnphongShader(Intersection* in, Brdf* brdf, Light* light, float ambient_intensity);
	void generateReflectedRay(Ray* iray, Ray* rray, Intersection* in);
	void calculateReflection(Vector3f* color, Ray* ray, Intersection* in, Vector3f k, float ambient_intensity, int depth);
};

#endif