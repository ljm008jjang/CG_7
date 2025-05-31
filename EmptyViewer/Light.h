#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Light {
public:
	Light(vec3 position) : pos(position), illumination(vec3(1,1,1)) {};

	vec3 pos;

	vec3 illumination;
};