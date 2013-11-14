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
		*color = {0,0,0};
		return;
	}

	/* calculate intersection for each object in scene and determine which is closest */
	float th, th_max = 0;
	bool hit;
	Intersection in;
	Intersection *in_max;
	Brdf brdf;
	Vector3f lcolor, temp;
	Ray lray;

	for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin() ; it != scene->primitives.end(); ++it) {
		hit = (*(*it)).intersect(ray, &th, &in);
		//std::cout<<"Hit - "<<hit<<"\n t = "<<th<<"\n local position - "<<in.local.position[0]<<','<<in.local.position[1]<<','<<in.local.position[2]<<"\n";
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

	for (std::vector<Light*>::iterator it = scene->lights.begin() ; it != scene->lights.end(); ++it) {
		(*it)->generateLightRay(in.local, lray, lcolor);

		bool block = false;
		/* check for blocking by other primitives */
		for (std::vector<GeoPrimitive*>::iterator it2 = scene->primitives.begin() ; it2 != scene->primitives.end(); ++it2) {
			if((**it2).intersect(lray, &th, &in)) {
				block = true;
				break;
			}
		}

		if(block) {
			*color = {0,0,0};
		}

		else {
			*color = in_max->primitive->shape->color;
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

	if(brdf.kr[0]>0 && brdf.kr[1]>0 && brdf.kr[2]>0) {
		Vector3f rm = (2*(in.local.normal.dot(ray.direction))*(in.local.normal) - ray.direction);
        Ray rray = {in.local.position, rm, 0, 1000}; /* Reflected ray */
        Vector3f tempColor;
        trace(rray, depth+1, &tempColor);
        temp = brdf.kr.array() * tempColor.array();
        *color += temp;
	}
}