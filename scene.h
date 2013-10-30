#include <Eigen/Dense>
using namespace Eigen;

/* Light source */
class Light
{
public:
	Vector3f position;
	Vector3f direction;
	Vector3f intensity; /* Intensity in each direction. Lies in 0-1. */
	Vector3f attenuation; /* Attenuation factor in each direction. If 0, no attenuation. Max value of 1. */
};

/* A ray of light */
struct Ray
{
	Vector3f origin;
	Vector3f direction;
};

/* Surface types */
enum SurfaceType {SMOOTH, ROUGH};	/* Currently supporting only smooth and rough surfaces. Later another class for detailed surface definition can be added */

/* Generic Object */
class Object
{
public:
	Vector3f position;
	SurfaceType surface;
};

/* Sphere object */
class Sphere: public Object
{
public:
	float radius;
	Sphere(Vector3f position, Surfacetype surface, float radius);
	~Sphere();
};

/* Sphere intersection */
struct SphereIntersection
{
	
};

/* To be added later
class Scene
{
public:
	Scene(Object *objects, Light *lights);
	~Scene();

};
*/