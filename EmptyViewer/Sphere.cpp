#pragma once
#include "Sphere.h"
#include "Ray.h"



//helped by Copilot
bool Sphere::intersect(Ray* ray, float tMin, float* tMax) {
	// Calculate the coefficients of the quadratic equation
	vec3 oc = ray->origin - Center;
	float a = glm::dot(ray->direction, ray->direction);
	float b = 2.0f * glm::dot(oc, ray->direction);
	float c = glm::dot(oc, oc) - Radius * Radius;
	// Calculate the discriminant
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false; // No intersection
	}
	// Calculate the intersection points
	float t1 = (-b - glm::sqrt(discriminant)) / (2.0f * a);
	float t2 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	// Check if the intersection points are within the bounds
	if (t1 < tMin || t1 > *tMax) {
		t1 = t2;
		if (t1 < tMin || t1 > *tMax) {
			return false; // Intersection is out of bounds
		}
	}

	*tMax = t1;
	return true;
}

vec3 Sphere::getNormal(vec3 point) {
	return normalize(point - Center);
}
