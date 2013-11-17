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
	if (!(camera && scene)) return;

	if(depth>threshold) {
		*color = {0,0,0};
		return;
	}

	/* calculate intersection for each object in scene and determine which is closest */
	float th, th_min = INFINITY;
	bool hit = false, per_hit = false;
	Intersection in;
	Intersection *in_min;
	Brdf* brdf;
	Vector3f lcolor, intensity;
	Ray lray;
	float ambient_intensity = 1.0;

	for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin(); it != scene->primitives.end(); it++) {
		th = INFINITY;
		per_hit = (*it)->intersect(ray, &th, &in);
		if (per_hit && th<th_min) {
			th_min = th;
			in_min = &in;
			hit = true;
		}
	}

	*color = {0,0,0};

	if (!hit) {
		//*color = {1,1,1};
		return;
	}


	brdf = in_min->primitive->getBrdf(in_min->local);
		

	for (std::vector<Light*>::iterator it = scene->lights.begin() ; it != scene->lights.end(); it++) {
		(*it)->generateLightRay(in_min->local, lray, lcolor);
		//std::cout<<lray.origin<<"\n"<<lray.direction;
		/* check for blocking by other primitives */
		for (std::vector<GeoPrimitive*>::iterator it2 = scene->primitives.begin() ; it2 != scene->primitives.end(); it2++) {
			per_hit = (*it2)->intersect(lray, &th, &in);
			if(per_hit) {
				break;
			}
		}

		
		Vector3f col;
		calculateReflection(&col, &ray, in_min, Vector3f(1.0, 1.0, 1.0), 1, depth);
		*color += col;

		if(per_hit) {
			*color += (brdf->ka * ambient_intensity);
			std::cout<<"brdf->ka = "<<brdf->ka<<"\nambient_intensity = "<<ambient_intensity<<"\ncolor = "<<*color;
		}
		else {
			*color += blinnphongShader(in_min, brdf, *it, ambient_intensity);
			//*color = in_min->primitive->shape->color;
			//std::cout<<*color<<"\nColor";
		}
	}

}

Vector3f RayTracer::blinnphongShader(Intersection* in, Brdf* brdf, Light* light, float ambient_intensity)
{
			Vector3f diffuse(0.0, 0.0, 0.0);
			Vector3f specular(0.0, 0.0, 0.0);
			Vector3f l,h,v;

			v = camera->position - in->local.position;
			v.normalize();
			l = light->position - in->local.position;
			l.normalize();
			h = v + l;
			h.normalize();

			diffuse = brdf->kd.array() * light->intensity.array() * l.dot(in->local.normal);
			//std::cout<<"\nDiffuse = "<<brdf->kd.array()<<"kd\n"<<l.dot(in->local.normal)<<"\n"<<diffuse;
			specular = brdf->ks.array() * light->intensity.array();
			//std::cout<<"\nSpecular = "<<specular;
			specular *= pow(h.dot(in->local.normal), brdf->s);
			//std::cout<<"\nSpecular = "<<specular;

			for (int i = 0; i<3; i++) {
				if (diffuse[i] < 0.0) diffuse[i] = 0.0;
				if (specular[i] < 0.0) specular[i] = 0.0;
			}
			
			//std::cout<<"\nka * intensity = "<<brdf->ka * ambient_intensity;

			return diffuse + specular + brdf->ka * ambient_intensity;
};

void RayTracer::generateReflectedRay(Ray* iray, Ray* rray, Intersection* in)
{
	//std::cout<<"\nINR.LOCAL.NORMAL 1= "<<in->local.normal<<"\n\n";
	rray->direction = iray->direction - in->local.normal * 2 * in->local.normal.dot(iray->direction);
	rray->direction.normalize();
	rray->origin = in->local.position;
}

void RayTracer::calculateReflection(Vector3f* color, Ray* ray, Intersection* in, Vector3f k, float ambient_intensity, int depth)
{
	std::cout<<"Depth = "<<depth;
	if(depth>threshold)
		return;

	Ray rray, lray;
	Vector3f lcolor, b;
	Intersection* in_min;
	Intersection in_r;
	generateReflectedRay(ray, &rray, in);
	std::cout<<rray.origin<<"\n";
	float rcos = 0.0;
	//std::cout<<"RRAY Direction = "<<rray.direction<<"\nINR.LOCAL.NORMAL 2= "<<in->local.normal<<"\n\n";
	rcos = rray.direction.dot(in->local.normal);
	float th, th_min = INFINITY;
	bool per_hit = false, hit = false;
	
	Brdf* brdf;
	GeoPrimitive* reflected_primitive;
	//std::cout<<"R_Angle = "<<rcos<<"\n";
	if (rcos>0.0){
		for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin(); it != scene->primitives.end(); it++) {
			if((*it) == in->primitive) {
				
				continue;
			}
			//else {
				//std::cout<<"!@#$^&*()(*&^$#@!@#$^&*(\n";
			//}
			th = INFINITY;
			per_hit = (*it)->intersect(rray, &th, &in_r);
			
			if (per_hit && th<th_min) {
				std::cout<<"Hit\n";
				th_min = th;
				in_min = &in_r;
				hit = true;
			}
		}
	}

	if(!hit){
		std::cout<<"No hit\n";
		return;
	}

	brdf = in_min->primitive->getBrdf(in_min->local);

	for (std::vector<Light*>::iterator it = scene->lights.begin(); it != scene->lights.end(); it++) {
		(*it)->generateLightRay(in_min->local, lray, lcolor);
		for (std::vector<GeoPrimitive*>::iterator it2 = scene->primitives.begin() ; it2 != scene->primitives.end(); it2++) {
			per_hit = (*it2)->intersect(lray, &th, &in_r);
			if(per_hit) {
				break;
			}
		}
		if(per_hit) {
			b = k.array() * brdf->ks.array() * brdf->ka.array() * ambient_intensity;
			std::cout<<"\nb = "<<b<<"\n";
			*color += b;
			std::cout<<"\nnew color = "<<*color<<"\n";
		}

		else {
			b = blinnphongShader(in_min, brdf, *it, 0.5);
			b = k.array() * brdf->ks.array() * b.array();
			*color += b;
		}
		calculateReflection(color, &lray, in_min, k.array() * brdf->ks.array(), ambient_intensity, depth+1);
	}

}

