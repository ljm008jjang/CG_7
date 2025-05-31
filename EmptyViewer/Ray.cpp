#pragma once
#include "Ray.h"


using namespace glm;

vec3 Ray::evaluate(float t) {
	return origin + direction * t;
}

