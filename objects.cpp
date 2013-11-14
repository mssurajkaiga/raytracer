#include "objects.h"
#include <math.h>

void Material::getBrdf(LocalGeo& local, Brdf* brdf)
{
	brdf = &b;;
}

Sphere::Sphere()
{
	center = Vector3f(0.0, 0.0, 0.0);
	color = Vector3f(0.0, 0.0, 0.0);
	radius = 1;
}

Sphere::Sphere(Vector3f cen, Vector3f col, float rad)
{
	center = cen;
	color = col;
	radius = rad;
}

bool Sphere::intersect(Ray& r, float* t_hit, LocalGeo* local)
{
	float A,B,C,D;
	A = r.direction.squaredNorm();
	B = r.direction.dot(2 * (r.origin - center));
	Vector3f t = r.origin - center;
	C = t.squaredNorm() - radius*radius;

	D = B * B - 4 * A * C; /* discriminant for calculating if intersection occurs or not */
	if (D<0) {
		return false;
	}

	else if (D==0) {
		*t_hit = -B/(2*A);
		return true;
	}

	else {
		float t1 = (-B-sqrt(D))/(2*A);
		float t2 = (-B+sqrt(D))/(2*A);

		if (t1<r.t_min && t2>=r.t_min) {
			if (t2<=r.t_max) {
				*t_hit = t2;
				(*local).position = r.origin + (*t_hit)*r.direction;
				/* calculate local.normal */
				return true;
			}

			return false;
		}

		else if (t1>=r.t_min && t1<=r.t_max) {
			*t_hit = t1;
			local->position = r.origin + (*t_hit)*r.direction;
			/* calculate local.normal */
			return true;
		}

		return false;	
	}

}
/*
Triangle::Triangle()
{
	vertices[0] = Vector3f(-1.0, 0.0, 0.0);
	vertices[1] = Vector3f(1.0, 0.0, 0.0);
	vertices[2] = Vector3f(0.0, 0.0, 0.0);
	color = Vector3f(0.0, 0.0, 0.0);
}

Triangle::Triangle(Vector3f* v, Vector3f col)
{
	for(int i =0; i<3; i++ ) vertices[i] = v[i];
	color = col;
}
*/

GeoPrimitive::GeoPrimitive()
{
	shape = new Sphere;
	material = new Material;
	material->b.kd = {0,0,0};
	material->b.ks = {0,0,0};
	material->b.ka = {0,0,0};
	material->b.kr = {0,0,0};
}

GeoPrimitive::GeoPrimitive(Shape* s, Material* m)
{
	shape = s;
	material = m;
}

bool GeoPrimitive::intersect(Ray& ray, float* t_hit, Intersection* in)
{
	//Ray oray = worldToObj*ray;
	LocalGeo olocal; /* object space local */
	//if(!shape->intersect(oray, t_hit, &olocal)) return false;
	if(!shape->intersect(ray, t_hit, &olocal)) return false;
	in->primitive = this;
	//in->local = objToWorld*olocal;
	in->local = olocal;
	return true;
}

void GeoPrimitive::getBrdf(LocalGeo& local, Brdf *brdf)
{
	material->getBrdf(local, brdf);
}