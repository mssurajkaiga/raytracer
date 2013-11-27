#include "objects.h"
#include <math.h>
#include <iostream>

Material::Material()
{
	b.kd = Vector3f(0,0,0);
	b.ks = Vector3f(0,0,0);
	b.ka = Vector3f(0,0,0);
	b.kr = Vector3f(0,0,0);
	b.s = 0.0;
	ri = 1.0;
}

Material::Material(Brdf brdf)
{
	b.kd = brdf.kd;
	b.ks = brdf.ks;
	b.ka = brdf.ka;
	b.kr = brdf.kr;
	b.s = brdf.s;
}

void Material::getBrdf(LocalGeo& local, Brdf* brdf)
{
	brdf = &b;
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
		*t_hit = INFINITY;
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
				local->position = r.origin + (*t_hit)*r.direction;
				/* calculate local.normal */
				local->normal = local->position - center;
				local->normal.normalize();
				return true;
			}
			*t_hit = INFINITY;
			return false;
		}

		else if (t1>=r.t_min && t1<=r.t_max) {
			*t_hit = t1;
			local->position = r.origin + (*t_hit)*r.direction;
			/* calculate local.normal */
			local->normal = local->position - center;
			local->normal.normalize();
			return true;
		}
		*t_hit = INFINITY;
		return false;	
	}

}

Triangle::Triangle()
{
	vertices[0] = Vector3f(-1.0, 0.0, 0.0);
	vertices[1] = Vector3f(1.0, 0.0, 0.0);
	vertices[2] = Vector3f(0.0, 0.0, 0.0);
	color = Vector3f(0.0, 0.0, 0.0);
	u = Vector3f(2.0, 0.0, 0.0);
	v = Vector3f(-1.0, 0.0, 0.0);
	normal = Vector3f(0.0, 0.0, -2.0);
}

Triangle::Triangle(Vector3f* v1, Vector3f col)
{
	for(int i =0; i<3; i++ ) vertices[i] = v1[i];
	/* precomputing u, v, normal for efficiency */
	u = vertices[1] - vertices[0];
	v = vertices[2] - vertices[0];
	normal = u.cross(v);
	normal.normalize();
	color = col;
}

bool Triangle::intersect(Ray& r, float* t_hit, LocalGeo* local)
{
	/* http://geomalgorithms.com/a06-_intersect-2.html */
	Vector3f w0, w;
	float s, r1, t, th, a, b;

	*t_hit = INFINITY;
	if (normal[0]==0 && normal[1]==0 && normal[2]==0) {
		return false;
	}

	w0 = r.origin - vertices[0];
	a = -normal.dot(w0);
	b = normal.dot(r.direction);
	if (fabs(b) < MIN_VAL) {
		return false;
	}

	th = a/b;

	if(th>r.t_max || th<r.t_min){
		return false;
	}

    float u2, uv, v2, wu, wv, D;
    u2 = u.squaredNorm();
    uv = u.dot(v);
    v2 = v.squaredNorm();
    w = r.origin + th * r.direction - vertices[0];
    wu = w.dot(u);
    wv = w.dot(v);
    D = uv * uv - u2 * v2;

    s = (uv * wv - v2 * wu) / D;
    if (s < 0.0 || s > 1.0)
        return false;
    t = (uv * wu - u2 * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)
        return false;
    
    *t_hit = th;
    local->position = r.origin + th * r.direction;
    local->normal = normal;
    return true;
}


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

Brdf* GeoPrimitive::getBrdf(LocalGeo& local)
{
	return &(material->b);
}