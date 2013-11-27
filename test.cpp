#include <iostream>
#include <cstdlib>
#include "raytracer.h"
#include "scene.h"
#include "objects.h"
#include "lights.h"
#include "utility.h"
#include "EasyBMP.h"
using namespace std;

int main(int argc, char **argv)
{
	if(argc!=2) {
		cout<<"Requires an argument specifying output file";
		exit(0);
	}
	
	int width = 600, height = 600;

	/* Material Test */
	Brdf br1 = {Vector3f(1.0, 0.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.1, 0.1, 0.1), Vector3f(0.0, 0.0, 0.0), 50.0};
	Material m1(br1);
	m1.ri = 1.52;
	Brdf br2 = {Vector3f(1.0, 1.0, 0.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.1, 0.1, 0.1), Vector3f(0.0, 0.0, 0.0), 50.0};
	Material m2(br2);
	m2.ri = 1.52;
	Brdf br3 = {Vector3f(0.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.1, 0.1, 0.1), Vector3f(0.0, 0.0, 0.0), 50.0};
	Material m3(br3);
	m3.ri = 1.52;
	Brdf br4 = {Vector3f(0.1, 0.1, 0.1), Vector3f(1.0, 1.0, 1.0), Vector3f(0.1, 0.1, 0.1), Vector3f(1.0, 1.0, 1.0), 50.0};
	Material m4(br4);
	m4.ri = 1.52;

	/* Primitive test */
	Sphere sp = Sphere(Vector3f(0, 0, 20), Vector3f(1,0,0), 3.0);
	GeoPrimitive g(&sp, &m1);
	Vector3f vertices[3];
	vertices[0] = Vector3f(-5, 5, 17);
	vertices[1] = Vector3f(-1, 4, 20);
	vertices[2] = Vector3f(-6, -1, 20);
	Triangle tr = Triangle(vertices, Vector3f(1,0,0));
	GeoPrimitive t(&tr, &m4);
	Brdf* temp;
	LocalGeo temp2;
	Sphere sp2 = Sphere(Vector3f(2, 2, 15), Vector3f(0,1,0), 1.0);
	GeoPrimitive g2(&sp2, &m2);
	Sphere sp3 = Sphere(Vector3f(2, -0.5, 15), Vector3f(0,0,1), 1.0);
	GeoPrimitive g3(&sp3, &m3);
	//Sphere sp4 = Sphere(Vector3f(0,-1008,10), Vector3f(0,0,1), 1000.0);
	//GeoPrimitive g4(&sp4, &m4);
	
	/* Light Test */
	DirectionalLight l = DirectionalLight(Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, 0.0, 0.0), Vector3f(0.57735, -0.57735, -0.57735));
	PointLight l1 = PointLight(Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, -100.0, 0.0));
	PointLight l2 = PointLight(Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, 100.0, 0.0));
	//DirectionalLight l2 = DirectionalLight(Vector3f(0.0, 0.0, 1.0), Vector3f(0.0, 0.0, 0.0), Vector3f(0.57735, -0.57735, -0.57735));
	/* Camera test */
	Sample s;
	s.x = 0;
	s.y = 0;
	Camera c(Vector3f(0, 0, 5), Vector3f(0,0,15), Vector3f(0,1,0), pi/3, pi/3, width, height);

	/* Sampler test */
	Sampler smp(width, height);
	smp.getSample();
	s = *(smp.sample);

	/* Scene test */
	Scene sc;
	sc.addPrimitiveToScene(&g);
	sc.addPrimitiveToScene(&g2);
	sc.addPrimitiveToScene(&g3);
	//sc.addPrimitiveToScene(&g4);
	sc.addPrimitiveToScene(&t);
	sc.addLightToScene(&l1);
	sc.addLightToScene(&l2);

	/* Generating output bitmap */
	BMP image;
	image.SetSize(width, height);
	image.SetBitDepth(24);

	/* Ray Tracer multi trace test*/
	Ray r;
	Vector3f color;
	RayTracer rt(10, &sc, &c);
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