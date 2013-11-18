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

	/* Materials */
	Brdf br1 = {Vector3f(0.4, 0.4, 0.4), Vector3f(0.8, 0.8, 0.8), Vector3f(0.1,0.1,0.1), Vector3f(0.2, 0.2, 0.5), 100.0};
	Material m1(br1);
	Brdf br2 = {Vector3f(0.2, 0.8, 0.6), Vector3f(0.2, 0.3, 0.5), Vector3f(0.1,0.1,0.1), Vector3f(0.5, 0.2, 0.3), 120.0};
	Material m2(br2);
	Brdf br3 = {Vector3f(0.1, 0.1, 0.7), Vector3f(0.3, 0.9, 0.3), Vector3f(0.2,0.1,0.1), Vector3f(0.7, 0.7, 0.7), 300.0};
	Material m3(br3);

	/* Primitives */
	Sphere sp = Sphere(Vector3f(10,0,10), Vector3f(1,0,0), 5.0);
	GeoPrimitive g(&sp, &m1);
	Brdf* temp;
	LocalGeo temp2;
	Sphere sp2 = Sphere(Vector3f(0,8,10), Vector3f(0,1,0), 4.0);
	GeoPrimitive g2(&sp2, &m2);
	Sphere sp3 = Sphere(Vector3f(-8,0,10), Vector3f(0,0,1), 5.0);
	GeoPrimitive g3(&sp3, &m3);
	
	/* Lights */
	PointLight l = PointLight(Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, 200.0, 0.0));
	PointLight l2 = PointLight(Vector3f(1.0, 1.0, 1.0), Vector3f(1.0, 1.0, 1.0), Vector3f(0.0, -100.0, 10.0));
	/* Camera*/
	Sample s;
	s.x = 0;
	s.y = 0;
	Camera c(Vector3f(0,0,-20), Vector3f(0,0,0), Vector3f(0,1,0), pi/3, pi/3, width, height);

	/* Sampler */
	Sampler smp(width, height);

	/* Scene */
	Scene sc;
	sc.addPrimitiveToScene(&g);
	sc.addPrimitiveToScene(&g2);
	sc.addPrimitiveToScene(&g3);
	sc.addLightToScene(&l);
	sc.addLightToScene(&l2);

	/* Generating output bitmap */
	BMP image;
	image.SetSize(width, height);
	image.SetBitDepth(24);

	/* Ray Trace */
	while(smp.getSample()) {
		c.generateRay(*(smp.sample), &r);
		color = {0.0, 0.0, 0.0};
		rt.trace(r, 0, &color);
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