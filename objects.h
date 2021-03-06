#ifndef OBJECTS_H
#define OBJECTS_H
#include "core.h"

/* Material Type */
class Material
{
public:
	Brdf b;
	float ri; /* refractive index */
	Material();
	Material(Brdf brdf);
	void getBrdf(LocalGeo& local, Brdf* brdf); /* not needed for now */
};

/* Abstract Shape */
class Shape
{
public:
	Vector3f color; /* to be removed, must be replaced with texture or something better */
	virtual bool intersect(Ray& r, float* t_hit, LocalGeo* local) = 0;
};

/* Sphere Shape */
class Sphere: public Shape
{
public:
	Vector3f center;
	float radius;
	Sphere();
	Sphere(Vector3f cen, Vector3f col, float rad);
	bool intersect(Ray& r, float* t_hit, LocalGeo* local);
};

/* Triangle Shape */
class Triangle: public Shape
{
public:
	Vector3f vertices[3], normal, u, v;
	Triangle();
	Triangle(Vector3f v1[], Vector3f col);
	bool intersect(Ray& r, float* t_hit, LocalGeo* local);
};

struct Intersection;
/* Abstract Primitive */
class Primitive
{
public:
	Shape* shape;
	Material* material;
	virtual bool intersect(Ray& ray, float* t_hit, Intersection* in) = 0;
	//virtual void getBrdf(LocalGeo& local, Brdf* brdf) = 0;
	virtual Brdf* getBrdf(LocalGeo& local) = 0;
};

/* Intersection */
struct Intersection
{
	LocalGeo local;
	Primitive* primitive;
};

/* Geometric Primitive */
class GeoPrimitive: public Primitive
{
public:
	GeoPrimitive();
	GeoPrimitive(Shape* s, Material* mat);
	bool intersect(Ray& ray, float* t_hit, Intersection* in);
	//void getBrdf(LocalGeo& local, Brdf* brdf); - don't know why this produces stupid values
	Brdf* getBrdf(LocalGeo& local);
};

#endif