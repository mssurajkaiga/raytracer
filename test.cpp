#include <iostream>
#include "raytracer.h"
#include "scene.h"
#include "objects.h"
#include "lights.h"
#include "utility.h"
#include "EasyBMP.h"
using namespace std;
#define pi 3.14159

int main(int argc, char **argv)
{

	int width = 300, height = 300;

	/* Ray test */
	Ray r;
	r.origin = Vector3f(0,0,0);
	r.direction = Vector3f(0,0,1);
	r.t_min = 0;
	r.t_max = 1000;

	/* Material Test */
	Material m;
	m.b = {Vector3f(0.5,0.1,0), Vector3f(0.1, 0.5, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, 0.0, 0.0)};

	/* Primitive test */
	Sphere sp = Sphere(Vector3f(0,0,10), Vector3f(1,0,0), 2.0);
	GeoPrimitive g(&sp, &m);
	Sphere sp2 = Sphere(Vector3f(5,0,5), Vector3f(0,1,0), 4.0);
	GeoPrimitive g2(&sp2, &m);
	float t_hit = 0;
	Intersection in;
	bool hit = g.intersect(r, &t_hit, &in);
	cout<<"Hit - "<<hit<<"\n t = "<<t_hit<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";

	/* Light Test */
	PointLight l = PointLight(Vector3f(0.5, 0.5, 0.5), Vector3f(1.0, 1.0, 1.0), Vector3f(10.0, 10.0, 10.0));
	/* Camera test */
	Sample s;
	s.x = 0;
	s.y = 0;
	Camera c(Vector3f(0,0,-2), Vector3f(0,0,0), Vector3f(0,1,0), pi/4, pi/4, width, height);

	/* Sampler test */
	Sampler smp(width, height);
	smp.getSample();
	s = *(smp.sample);
	cout<<"Sample x = "<<s.x<<" y = "<<s.y<<"\n";

	/* Intersection test */
	c.generateRay(s, &r);
	t_hit = 0;
	hit = g.intersect(r, &t_hit, &in);
	cout<<"Ray - \n Origin = "<<r.origin<<"\nDirection = "<<r.direction<<"\n";
	cout<<"Hit - "<<hit<<"\n t = "<<t_hit<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";

	/* Scene test */
	Scene sc;
	sc.addPrimitiveToScene(&g);
	sc.addLightToScene(&l);

	/* Ray Tracer single trace test */
	cout<<"\n --------- RayTracer Single Trace----------\n";
	Vector3f color;
	RayTracer rt(1, &sc, &c);
	rt.trace(r, 1, &color);
	cout<<color;

	/* Generating output bitmap */
	BMP image;
	image.SetSize(width, height);
	image.SetBitDepth(24);

	/* Ray Tracer multi trace test*/
	cout<<"\n --------- RayTracer Multi Trace----------\n";
	while(smp.getSample()) {
		c.generateRay(*(smp.sample), &r);
		//cout<<"Ray - \n Origin = "<<r.origin<<"\nDirection = "<<r.direction<<"\n";
		rt.trace(r, 1, &color);
		cout<<color<<"\n\n";
		image(smp.sample->x, smp.sample->y)->Red = color[0]*255;
		image(smp.sample->x, smp.sample->y)->Green = color[1]*255;
		image(smp.sample->x, smp.sample->y)->Blue = color[2]*255;
	}
	
	image.WriteToFile("output.bmp");

	return 0;
}