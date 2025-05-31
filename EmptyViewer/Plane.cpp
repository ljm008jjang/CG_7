#pragma once
#include "Plane.h"
#include "Ray.h"

using namespace glm;
using namespace std;


//helped by Copilot
bool Plane::intersect(Ray* ray, float tMin, float* tMax) {
    // Calculate the denominator of the intersection formula
    float denom = dot(Normal, ray-> direction);
    if (abs(denom) < 1e-6) {
        return false; // Ray is parallel to the plane
    }

    // Calculate the intersection point
    float t = -(dot(Normal, ray->origin) - PlaneD) / denom;
    if (t < tMin || t > *tMax) {
        return false; // Intersection is out of bounds
    }

	*tMax = t;
    return true;
}

vec3 Plane::getNormal(vec3 point) {
    return normalize(Normal);
}
