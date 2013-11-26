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
		return;
	}


	brdf = in_min->primitive->getBrdf(in_min->local);
		

	for (std::vector<Light*>::iterator it = scene->lights.begin() ; it != scene->lights.end(); it++) {
		(*it)->generateLightRay(in_min->local, lray, lcolor);
		/* check for blocking by other primitives */
		for (std::vector<GeoPrimitive*>::iterator it2 = scene->primitives.begin() ; it2 != scene->primitives.end(); it2++) {
			per_hit = (*it2)->intersect(lray, &th, &in);
			if(per_hit) {
				break;
			}
		}

		
		
		calculateReflection(color, &ray, in_min, Vector3f(1.0, 1.0, 1.0), 1, depth);
		calculateRefraction(color, &ray, in_min, Vector3f(1.0, 1.0, 1.0), 1, depth);
		

		if(per_hit) {
			*color += (brdf->ka * ambient_intensity);
		}
		else {
			*color += blinnphongShader(in_min, brdf, *it, ambient_intensity);
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
			specular = brdf->ks.array() * light->intensity.array();
			specular *= pow(h.dot(in->local.normal), brdf->s);

			for (int i = 0; i<3; i++) {
				if (diffuse[i] < 0.0) diffuse[i] = 0.0;
				if (specular[i] < 0.0) specular[i] = 0.0;
			}

			return diffuse + specular + brdf->ka * ambient_intensity;
};

void RayTracer::generateReflectedRay(Ray* iray, Ray* rray, Intersection* in)
{
	rray->direction = iray->direction - in->local.normal * 2 * in->local.normal.dot(iray->direction);
	rray->direction.normalize();
	rray->origin = in->local.position + 0.0001 * rray->direction;
}

void RayTracer::generateRefractedRay(Ray* iray, Ray* rray, Intersection* in)
{
	Vector3f normal = in->local.normal.cross(iray->direction);

	Matrix4f rot = Matrix4f::Zero(); 
	VectorXf input  = iray->direction;
	input.conservativeResize(4);
	input[3] = 1.0;
	VectorXf output = Vector4f::Zero();

	float u = in->local.normal[0];
	float v = in->local.normal[1];
	float w = in->local.normal[2];
	float angle = pi/2-acosf(iray->direction.dot(in->local.normal));
	angle -= asinf(in->primitive->material->ri * sinf(angle));

	float mag = in->local.normal.squaredNorm();
	float u2 = u * u;     
	float v2 = v * v;     
	float w2 = w * w;       
	rot(0,0) = (u2 + (v2 + w2) * cos(angle)) / mag;
	rot(0,1) = (u * v * (1 - cos(angle)) - w * sqrt(mag) * sin(angle)) / mag;
	rot(0,2) = (u * w * (1 - cos(angle)) + v * sqrt(mag) * sin(angle)) / mag;
	rot(1,0) = (u * v * (1 - cos(angle)) + w * sqrt(mag) * sin(angle)) / mag;
	rot(1,1) = (v2 + (u2 + w2) * cos(angle)) / mag;
	rot(1,2) = (v * w * (1 - cos(angle)) - u * sqrt(mag) * sin(angle)) / mag;
	rot(2,0) = (u * w * (1 - cos(angle)) - v * sqrt(mag) * sin(angle)) / mag;
	rot(2,1) = (v * w * (1 - cos(angle)) + u * sqrt(mag) * sin(angle)) / mag;
	rot(2,2) = (w2 + (u2 + v2) * cos(angle)) / mag;
	rot(3,3) = 1.0;

	output = rot * input;

	rray->origin = in->local.position;
	output.conservativeResize(3);
	rray->direction = output;
}

void RayTracer::calculateReflection(Vector3f* color, Ray* ray, Intersection* in, Vector3f k, float ambient_intensity, int depth)
{
	
	if(depth>threshold)
		return;

	Ray rray, lray;
	Vector3f lcolor, b;
	Intersection* in_min;
	Intersection in_r;
	generateReflectedRay(ray, &rray, in);
	//std::cout<<rray.origin<<"\n";
	float rcos = 0.0;

	rcos = rray.direction.dot(in->local.normal);
	float th, th_min = INFINITY;
	bool per_hit = false, hit = false;
	
	Brdf* brdf;
	GeoPrimitive* reflected_primitive;
	
	if (rcos>0.0){
		for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin(); it != scene->primitives.end(); it++) {
			if((*it) == in->primitive) {
				
				continue;
			}

			th = INFINITY;
			per_hit = (*it)->intersect(rray, &th, &in_r);
			
			if (per_hit && th<th_min) {
				th_min = th;
				in_min = &in_r;
				hit = true;
			}
		}
	}

	if(!hit){
		return;
	}

	brdf = in_min->primitive->getBrdf(in_min->local);
	int i=0;
	for (std::vector<Light*>::iterator it = scene->lights.begin(); it != scene->lights.end(); it++) {
		(*it)->generateLightRay(in_min->local, lray, lcolor);
		if (i++==1) std::cout<<"i=1\t";
		for (std::vector<GeoPrimitive*>::iterator it2 = scene->primitives.begin() ; it2 != scene->primitives.end(); it2++) {
			per_hit = (*it2)->intersect(lray, &th, &in_r);
			if(per_hit) {
				break;
			}
		}
		if(per_hit) {
			b = k.array() * brdf->ks.array() * brdf->ka.array() * ambient_intensity;
			*color += b;
		}

		else {
			b = blinnphongShader(in_min, brdf, *it, 0.5);
			b = k.array() * brdf->ks.array() * b.array();
			*color += b;
		}
		calculateReflection(color, &rray, in_min, k.array() * brdf->ks.array(), ambient_intensity, depth+1);
	}

}

void RayTracer::calculateRefraction(Vector3f* color, Ray* ray, Intersection* in, Vector3f k, float ambient_intensity, int depth)
{
	if(depth>threshold)
		return;

	Ray rray, lray, tray;
	Vector3f lcolor, b;
	Intersection* in_min;
	Intersection in_r;
	generateRefractedRay(ray, &tray, in);
	generateRefractedRay(&tray, &rray, in);
	float rcos = 0.0;

	rcos = rray.direction.dot(in->local.normal);
	float th, th_min = INFINITY;
	bool per_hit = false, hit = false;
	
	Brdf* brdf;
	GeoPrimitive* reflected_primitive;

	if (rcos>0.0){
		for (std::vector<GeoPrimitive*>::iterator it = scene->primitives.begin(); it != scene->primitives.end(); it++) {
			if((*it) == in->primitive) {
				
				continue;
			}

			th = INFINITY;
			per_hit = (*it)->intersect(rray, &th, &in_r);
			
			if (per_hit && th<th_min) {
				//std::cout<<"Hit\n";
				th_min = th;
				in_min = &in_r;
				hit = true;
			}
		}
	}

	if(!hit){
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
			*color += b;
		}

		else {
			b = blinnphongShader(in_min, brdf, *it, 0.5);
			b = k.array() * brdf->ks.array() * b.array();
			*color += b;
		}
		calculateReflection(color, &rray, in_min, k.array() * brdf->ks.array(), ambient_intensity, depth+1);
	}

}