#include "raytracer.h"

RayTracer::RayTracer()
{
	threshold = 1;
	scene = NULL;
	camera = NULL;
}

RayTracer::RayTracer(int t, Scene* s, Camera* c)
{
	threshold = t;
	scene = s;
	camera = c;
}

void RayTracer::trace(Ray& ray, int depth, Vector3f* color)
{
	if (!(camera || scene)) return;

	if(depth>threshold) {
		*color = {1,1,0};
		return;
	}

	/* calculate intersection for each object in scene and determine which is closest */
	float th, th_min = INFINITY;
	bool hit = false, per_hit = false;
	Intersection in;
	Intersection *in_min;
	Brdf brdf;
	Vector3f lcolor, temp;
	Ray lray;
	int i = 0;

	for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin(); it != scene->primitives.end(); it++) {
		th = INFINITY;
		per_hit = (*it)->intersect(ray, &th, &in);
		//std::cout<<"\nHit - "<<hit<<"\n t = "<<th<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";
		if (per_hit && th<th_min) {
			th_min = th;
			in_min = &in;
			std::cout<<"Hit = "<<per_hit<<"\t"<<i<<"\t";
			hit = true;
		}
		i++;
	}
	std::cout<<th_min<<"\n";
	//std::cout<<"\n";

	if (!hit) {
		*color = {1,1,1};
		return;
	}

	else {
		in_min->primitive->getBrdf(in.local, &brdf);
	}

	for (std::vector<Light*>::iterator it = scene->lights.begin() ; it != scene->lights.end(); ++it) {
		(*it)->generateLightRay(in_min->local, lray, lcolor);
		bool block = false;
		/* check for blocking by other primitives */
		for (std::vector<GeoPrimitive*>::iterator it2 = scene->primitives.begin() ; it2 != scene->primitives.end(); ++it2) {
			std::cout<<"Counter\t\t";
			if((*it2)->intersect(lray, &th, &in)) {
				block = true;
				break;
			}
		}
		if(block) *color = {1,1,1};
		if(!block) {
			*color = in_min->primitive->shape->color;
		}
		/*
		if(!block) {

			temp = (*it)->ambient;
			temp = brdf.ka.array() * temp.array();
			*color += temp;
			temp = brdf.kd * in.local.normal.dot((*it)->position - in.local.position);
			*color += temp;
			Vector3f rm = (2*in.local.normal.dot((*it)->position - in.local.position)*(in.local.normal) - ((*it)->position - in.local.position));
			*/
			/* diffuse part to be computed later
			float temp2 = rm.dot(in.local.position - camera->position);
			temp = pow(temp, brdf.kr) * 1;
			*color += brdf.ks * temp;
			
		}
		*/

	}

	//std::cout<<"\n t_min = "<<th_min;

	if(brdf.kr[0]>0 && brdf.kr[1]>0 && brdf.kr[2]>0) {
		Vector3f rm = (2*(in.local.normal.dot(ray.direction))*(in.local.normal) - ray.direction);
        Ray rray = {in.local.position, rm, 0, 1000}; /* Reflected ray */
        Vector3f tempColor;
        trace(rray, depth+1, &tempColor);
        temp = brdf.kr.array() * tempColor.array();
        *color += temp;
	}
}