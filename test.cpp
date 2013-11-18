#include <iostream>
#include "raytracer.h"
#include "scene.h"
#include "objects.h"
#include "lights.h"
#include "utility.h"
#include "EasyBMP.h"
using namespace std;

int main(int argc, char **argv)
{

	int width = 600, height = 600;

	/* Ray test */
	Ray r;
	r.origin = Vector3f(0,0,30);
	r.direction = Vector3f(0,0,1);
	r.t_min = 0;
	r.t_max = 1000;

	/* Material Test */
	Brdf br1 = {Vector3f(0.4, 0.4, 0.4), Vector3f(0.8, 0.8, 0.8), Vector3f(0.1,0.1,0.1), Vector3f(0.2, 0.2, 0.5), 100.0};
	//cout<<"\nbr.ka = "<<br1.ka;
	Material m1(br1);
	Brdf br2 = {Vector3f(0.2, 0.8, 0.6), Vector3f(0.7, 0.6, 0.5), Vector3f(0.5,0.5,0.5), Vector3f(0.5, 0.2, 0.3), 120.0};
	Material m2(br2);
	Brdf br3 = {Vector3f(0.1, 0.1, 0.7), Vector3f(0.3, 0.9, 0.3), Vector3f(0.2,0.1,0.1), Vector3f(0.7, 0.7, 0.7), 300.0};
	Material m3(br3);

	/* Primitive test */
	Sphere sp = Sphere(Vector3f(10,0,10), Vector3f(1,0,0), 5.0);
	GeoPrimitive g(&sp, &m1);
	Brdf* temp;
	LocalGeo temp2;
	temp = g.getBrdf(temp2);
	cout<<"new temp brdf = "<<temp->ka<<"\n";
	Sphere sp2 = Sphere(Vector3f(-5,0,10), Vector3f(0,1,0), 5.0);
	GeoPrimitive g2(&sp2, &m2);
	Sphere sp3 = Sphere(Vector3f(0,-208,10), Vector3f(0,0,1), 200.0);
	GeoPrimitive g3(&sp3, &m3);
	float t_hit = 0;
	Intersection in;
	bool hit = g.intersect(r, &t_hit, &in);
	cout<<"Hit - "<<hit<<"\n t = "<<t_hit<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";

	/* Light Test */
	PointLight l = PointLight(Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, 200.0, 10.0));
	/* Camera test */
	Sample s;
	s.x = 0;
	s.y = 0;
	Camera c(Vector3f(0,0,-20), Vector3f(0,0,0), Vector3f(0,1,0), pi/3, pi/3, width, height);

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
	sc.addPrimitiveToScene(&g2);
	sc.addPrimitiveToScene(&g3);
	sc.addLightToScene(&l);

	/* Ray Tracer single trace test */
	cout<<"\n --------- RayTracer Single Trace----------\n";
	Vector3f color;
	RayTracer rt(10, &sc, &c);
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
		color = {0.0, 0.0, 0.0};
		//cout<<"X = "<<smp.sample->x<<" Y = "<<smp.sample->y<<"\n";
		rt.trace(r, 0, &color);
		//cout<<"\n-----Color = "<<color<<"\n\n";
		for (int i = 0; i<3; i++) {
			if (color[i]>1.0) color[i] = 1.0;
			else if (color[i]<0.0) color[i] = 0.0;
		}
		image(smp.sample->x, smp.sample->y)->Red = color[0]*255;
		image(smp.sample->x, smp.sample->y)->Green = color[1]*255;
		image(smp.sample->x, smp.sample->y)->Blue = color[2]*255;
	}
	
	image.WriteToFile(argv[1]);

	return 0;
}