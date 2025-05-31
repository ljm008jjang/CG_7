#pragma once
#include <glm/glm.hpp>
#include "Surface.h"

using namespace glm;

class Plane : public Surface {
public:
    Plane(vec3 Ka, vec3 Kd, vec3 Ks,float SpecularPower, vec3 normal, float planeD) : Normal(normal), PlaneD(planeD), Surface(Ka, Kd, Ks, SpecularPower) {}

    // -2.0f
    float PlaneD;

public:
    //helped by Copilot
    virtual bool intersect(Ray* ray, float tMin, float* tMax) override;

    virtual vec3 getNormal(vec3 point);

private:
    // Plane equation: Ax + By + Cz = D
    vec3 Normal; // ¹ý¼± º¤ÅÍ (a, b, c)
};