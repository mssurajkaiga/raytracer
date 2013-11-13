#include <iostream>
#include "raytracer.h"
#include "scene.h"
#include "objects.h"
#include "lights.h"
#include "utility.h"
using namespace std;
#define pi 3.14159

int main(int argc, char **argv)
{
	Ray r;
	r.origin = Vector3f(0,0,0);
	r.direction = Vector3f(0,0,1);
	r.t_min = 0;
	r.t_max = 1000;

	Sphere sp = Sphere(Vector3f(0,0,20), Vector3f(1,0,0), 20.0);
	GeoPrimitive g(&sp,NULL);
	float t_hit = 0;
	Intersection in;
	bool hit = g.intersect(r, &t_hit, &in);
	cout<<"Hit - "<<hit<<"\n t = "<<t_hit<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";

	Sample s;
	s.x = 0;
	s.y = 0;
	Camera c(Vector3f(0,0,-2), Vector3f(0,0,0), Vector3f(0,1,0), pi/4, pi/4, 800, 600);

	/* Sampler test */
	Sampler smp;
	smp.getSample();
	s = *(smp.sample);
	cout<<"Sample x = "<<s.x<<" y = "<<s.y<<"\n";

	c.generateRay(s, &r);
	t_hit = 0;
	hit = g.intersect(r, &t_hit, &in);
	cout<<"Ray - \n Origin = "<<r.origin<<"\nDirection = "<<r.direction<<"\n";
	cout<<"Hit - "<<hit<<"\n t = "<<t_hit<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";

	/* Scene testing */
	Scene sc;
	sc.addPrimitiveToScene(&g);

	/* Ray Tracer testing */
	cout<<"\n --------- RayTracer ----------\n";
	Vector3f color;
	RayTracer rt(1, &sc);
	rt.trace(r, 1, &color);

	return 0;
}