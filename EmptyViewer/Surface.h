#pragma once
#include <glm/glm.hpp>
#include <vector>

class Ray;
class Light;
class Scene;

using namespace std;
using namespace glm;


class Surface {
public:
	Surface(vec3 Ka, vec3 Kd, vec3 Ks, int SpecularPower) : ka(Ka), kd(Kd), ks(Ks), specularPower(SpecularPower){}

	virtual bool intersect(Ray* ray, float tMin, float* tMax) = 0;

	virtual vec3 getNormal(vec3 point) = 0;

	vec3 shade(Ray* ray, vec3 point, vec3 normal, vector<Light*> light, Scene* scene);

protected:
	vec3 ka, kd, ks;

	float Ia = 1;
	int specularPower;
};