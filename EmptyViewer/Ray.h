#pragma once
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

class Ray {
public:
	Ray(const vec3& origin, const vec3& direction) : origin(origin), direction(normalize(direction)) {}

	vec3 evaluate(float t);

	vec3 origin;
	vec3 direction;
};