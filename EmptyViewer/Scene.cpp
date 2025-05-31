#pragma once
#include "Scene.h"
#include "Surface.h"
#include "Light.h"
#include "Ray.h"

using namespace glm;
using namespace std;


void Scene::clear() {
	surfaces.clear();
	lights.clear();
}

//Add Object
void Scene::addSurface(Surface* surface) {
	surfaces.push_back(surface);
}

void Scene::addLight(Light* light) {
	lights.push_back(light);
}

//Check intersected Objects and return color of pixel
vec3 Scene::trace(Ray* ray, float tMin, float tMax) {
	//set default color
	vec3 color = vec3(0.0f, 0.0f, 0.0f);

	float closestT = tMax;
	Surface* closestSurface = nullptr;

	for (int i = 0; i < surfaces.size(); i++) {
		if (surfaces[i]->intersect(ray, tMin, &closestT)) {
			//closestT = tMin; // already Updated closest intersection point in intersect function
			closestSurface = surfaces[i];
		}
	}

	if (closestSurface != nullptr) {
		vec3 point = ray->evaluate(closestT);
		vec3 normal = closestSurface->getNormal(point);
		color = closestSurface->shade(ray, point, normal, lights, this);
	}

	return color;
}

bool Scene::isShadowed(Ray* shadRay, float lightDistance) {
	for (Surface* surface : surfaces) {
		float tMax = lightDistance;
		if (surface->intersect(shadRay, 0.001f, &tMax)) {
			return true; // Shadow ray is blocked
		}
	}
	return false; // No intersection, not in shadow
}
