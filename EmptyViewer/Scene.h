#pragma once
#include <vector>
#include <glm/glm.hpp>

class Surface;
class Light;
class Ray;

using namespace std;
using namespace glm;

class Scene {
	//Scene Objects
	vector<Surface*> surfaces;
	vector < Light*> lights;

public:
	void clear();

	//Add Object
	void addSurface(Surface* surface);
	void addLight(Light* light);

	//Check intersected Objects and return color of pixel
	vec3 trace(Ray* ray, float tMin, float tMax);

	bool isShadowed(Ray* shadRay, float lightDistance);
};