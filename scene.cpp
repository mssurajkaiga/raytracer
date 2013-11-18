#include "scene.h"

Sampler::Sampler()
{
	x_max = 800;
	y_max = 600;
	sample = new Sample;
}

Sampler::Sampler(int x, int y)
{
	x_max = x;
	y_max = y;
	sample = new Sample;
}

bool Sampler::getSample(Sample* s)
{
	/*currently returns at one sample per pixel */
	if ((*s).x<x_max-1) {
		(*s).x++;
		return true;
	}
	else if ((*s).y<y_max-1) {
		(*s).x = 0;
		(*s).y++;
		return true;
	}
	else return false;
}

bool Sampler::getSample()
{
	return getSample(sample);
}

Camera::Camera(Vector3f pos, Vector3f lAt, Vector3f u_, float fx, float fy, int wid = 800, int hei = 600)
{
	position = pos;
	lookAt = lAt;
	up = u_;
	fov_x = fx;
	fov_y = fy;
	width = wid;
	height = hei;

	w = position-lookAt;
	w.normalize();
	u = up.cross(w);
	u.normalize();
	v = w.cross(u);
}

void Camera::generateRay(Sample& sample, Ray* ray)
{
	float a,b;
	a = tan(fov_x/2) * (sample.x - width/2) * 2 / width;
	b = tan(fov_y/2) * (height/2 - sample.y) * 2 / height;
	/* can precompute tan, height/2 and width/2 values in constructor to speed up */
	ray->origin = position;
	ray->direction = a*u + b*v - w;
	ray->direction.normalize();
}

Scene::Scene(std::vector<GeoPrimitive*> p, std::vector<Light*> l)
{
	primitives = p;
	lights = l;
}

void Scene::addPrimitiveToScene(GeoPrimitive* p)
{
	primitives.push_back(p);
}

void Scene::addLightToScene(Light* l)
{
	lights.push_back(l);
}

Scene::Scene()
{
	
}