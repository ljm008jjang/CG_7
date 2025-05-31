#pragma once
#include "Surface.h"

class Ray;

using namespace glm;


class Sphere : public Surface {
public:
	Sphere(vec3 Ka, vec3 Kd, vec3 Ks, float SpecularPower, vec3 center, float radius) : Center(center), Radius(radius), Surface(Ka, Kd, Ks, SpecularPower) {}

private:
	vec3 Center;
	float Radius;


public:
	//helped by Copilot
	virtual bool intersect(Ray* ray, float tMin, float* tMax) override;

	virtual vec3 getNormal(vec3 point);
};