#include "raytracer.h"

RayTracer::RayTracer()
{
	threshold = 1;
	scene = NULL;
}

RayTracer::RayTracer(int t, Scene* s)
{
	threshold = t;
	scene = s;
}

void RayTracer::trace(Ray& ray, int depth, Vector3f* color)
{
	if(depth>threshold) {
		*color = {0,0,0};
		return;
	}

	/* calculate intersection for each object in scene and determine which is closest */
	float th, th_max = 0;
	bool hit;
	Intersection in;
	Intersection *in_max;
	Brdf brdf;

	for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin() ; it != scene->primitives.end(); ++it) {
		hit = (*(*it)).intersect(ray, &th, &in);
		std::cout<<"\nScene hit = "<<hit;
		std::cout<<"Hit - "<<hit<<"\n t = "<<th<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";
		if (th>th_max) {
			th_max = th;
			in_max = &in;
		}
	}

	if (!hit) {
		*color = {0,0,0};
		return;
	}

	else {
		in.primitive->getBrdf(in.local, &brdf);
	}

	
}