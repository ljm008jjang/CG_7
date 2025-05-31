#pragma once

#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Include this header for glm::value_ptr
#include <glm/gtx/string_cast.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Camera.h"
#include "Plane.h"
#include "Light.h"
#include "sphere_scene.cpp"

using namespace glm;

// -------------------------------------------------
// Global Variables
// -------------------------------------------------
int Width = 512;
int Height = 512;
std::vector<float> OutputImage;
// -------------------------------------------------
//Camera camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), 45.0f, (float)Width / (float)Height, -0.1f, -1000.0f);
Camera camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), 45.0f, (float)Width / (float)Height, -0.1f, -1000.0f);
std::vector<float> DepthBuffer;

sphere_scene sphere(vec3(0, 1, 0), vec3(0, 0.5f, 0), vec3(0.5f, 0.5f, 0.5f), 32, vec3(0, 0, -7), 2);
std::vector<sphere_scene> sceneObjects;

Light light(vec3(-4, 4, -3));

//helped by chatGPT
void rasterize_triangle(const vec3& screen0, const vec3& screen1, const vec3& screen2, const vec3& v0, const vec3& v1, const vec3& v2, const float& invW0, const float& invW1, const float& invW2, const vec3& normal, const Light& light, const sphere_scene& sphere) {
	// cross product
	auto edge = [](const vec2& a, const vec2& b, const vec2& c) {
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
		};

	vec2 p0 = screen0.xy();
	vec2 p1 = screen1.xy();
	vec2 p2 = screen2.xy();

	float area = edge(p0, p1, p2);
	if (area == 0) return;

	int minX = std::max(0, (int)floor(min({ p0.x, p1.x, p2.x })));
	int maxX = std::min(Width - 1, (int)ceil(max({ p0.x, p1.x, p2.x })));
	int minY = std::max(0, (int)floor(min({ p0.y, p1.y, p2.y })));
	int maxY = std::min(Height - 1, (int)ceil(max({ p0.y, p1.y, p2.y })));

	float invArea = 1.0f / area;
	vec2 start(minX + 0.5f, minY + 0.5f);
	// alpha, beta, gamma는 모두 삼각형의 넓이의 비율과 관련이 있다.
	// 주석처리된 공식과 사용된 공식은 같은 의미.
	// 수업 자료는 후자 공식이지만, 코드의 편의성을 위해 앞 공식을 사용한다.
	float startBeta = edge(p2, p0, start) * invArea; //(((p0.y - p2.y) * minX + (p2.x - p0.x) * minY + p0.x * p2.y - p2.x * p0.y)) / ((p0.y - p2.y) * p1.x + (p2.x - p0.x) * p1.y + p0.x * p2.y - p2.x * p0.y);
	float startGamma = edge(p0, p1, start) * invArea;//(((p0.y - p1.y) * minX + (p1.x - p0.x) * minY + p0.x * p1.y - p1.x * p0.y)) / ((p0.y - p1.y) * p2.x + (p1.x - p0.x) * p2.y + p0.x * p1.y - p1.x * p0.y);
	float n = (minX - minX) + 1;

	float betaX = (p0.y - p2.y) * invArea;
	float gammaX = (p1.y - p0.y) * invArea;

	float betaY = ((p2.x - p0.x) * invArea);
	float gammaY = ((p0.x - p1.x) * invArea);

	float beta = startBeta;
	float gamma = startGamma;


	vec3 point = (v0 + v1 + v2) / 3.0f;

	vec3 v = normalize(camera.e - point);  // view direction
	vec3 l = normalize(light.pos - point);  // light direction

	vec3 h = normalize(v + l);  // half vector

	// Phong shading
	vec3 La = sphere.Ka * 0.2f;
	vec3 Ld = sphere.Kd * light.illumination * std::max(0.0f, dot(normal, l));
	vec3 Ls = sphere.Ks * light.illumination * pow(std::max(0.0f, dot(normal, h)), float(sphere.p));

	vec3 result = La + Ld + Ls;

	float colorGamma = 2.2f;
	float invGamma = 1.0f / colorGamma;

	// Apply gamma correction
	result.r = pow(result.r, invGamma);
	result.g = pow(result.g, invGamma);
	result.b = pow(result.b, invGamma);

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			if (beta > 0 && gamma > 0 && beta + gamma < 1) {

				// 보간 (β, γ는 이미 화면 좌표 기준에서 구해졌다고 가정)
				float invZ = beta * invW0 + gamma * invW1 + (1 - beta - gamma) * invW2;
				float z = 1.0f / invZ; // perspective-correct z
				//float z = (screen0.z * beta + screen1.z * gamma + screen2.z * (1 - beta - gamma));
				int idx = y * Width + x;
				if (z < DepthBuffer[idx]) {
					DepthBuffer[idx] = z;


					OutputImage[idx * 3 + 0] = result.r;
					OutputImage[idx * 3 + 1] = result.g;
					OutputImage[idx * 3 + 2] = result.b;
				}
			}
			// x에 대한 β, γ 갱신
			beta += betaX;
			gamma += gammaX;
		}

		// y에 대한 β, γ 갱신
		startBeta += betaY;
		startGamma += gammaY;

		// 다음 행을 위한 초기값 설정
		beta = startBeta;
		gamma = startGamma;

		// 수업 자료의 sudo코드를 그대로 사용하면 오류 발생.
		// 부동소수점이 문제이지 않을까 싶다.
	}
}



void render()
{
	//Create our image. We don't want to do this in 
	//the main loop since this may be too slow and we 
	//want a responsive display of our beautiful image.
	//Instead we draw to another buffer and copy this to the 
	//framebuffer using glDrawPixels(...) every refresh
	OutputImage.clear();
	DepthBuffer.clear();
	sceneObjects.clear();

	OutputImage.assign(Width * Height * 3, 0.0f);
	DepthBuffer.assign(Width * Height, 1e9f);
	sceneObjects.push_back(sphere);

	for (sphere_scene& sceneObject : sceneObjects)
	{
		sceneObject.clear();
		sceneObject.create_scene();
	}

	// 1. Model Transform
	//mat4 model = translate(mat4(1.0f), vec3(0, 0, -7)) * scale(mat4(1.0f), vec3(2.0f));

	// 2. View Transform (Identity)
	mat4 view = camera.getViewMatrix();

	// 3. Perspective Projection (사용자 정의)
	// 직접 만든 Perspective 행렬 (OpenGL 호환 버전)
	mat4 proj = camera.getProjectionMatrix();

	mat4 VP = proj * view;

	for (sphere_scene& sceneObject : sceneObjects) {
		mat4 model = sceneObject.get_model_transform();
		mat4 MVP = VP * model;
		for (int i = 0; i < sceneObject.gNumTriangles; ++i) {

			vec3 v0, v1, v2;
			vec3 screen0, screen1, screen2;
			float invW0, invW1, invW2;

			//삼각형 점 좌표, 화면 좌표를 가져온다
			sceneObject.process_triangle(MVP, model, Width, Height, &v0, &v1, &v2, &screen0, &screen1, &screen2, &invW0, &invW1, &invW2, i);

			//절두체 체크
			if (camera.isInFrustum(v0, v1, v2) == false) {
				continue;
			}

			//back face culling
			vec3 faceNormal = normalize(cross(v1 - v0, v2 - v0));
			vec3 center = (v0 + v1 + v2) / 3.0f;
			vec3 viewDir = normalize(camera.e - center);

			if (dot(faceNormal, viewDir) < 0) {
				continue; // back-face culling
			}
			//레스터링
			rasterize_triangle(screen0, screen1, screen2, v0, v1, v2, invW0, invW1, invW2, faceNormal, light, sceneObject);
		}
	}



	//for (int j = 0; j < Height; ++j) 
	//{
	//	for (int i = 0; i < Width; ++i) 
	//	{
	//		// ---------------------------------------------------
	//		// --- Implement your code here to generate the image
	//		// ---------------------------------------------------
	//	}
	//}
}


void resize_callback(GLFWwindow*, int nw, int nh)
{
	//This is called in response to the window resizing.
	//The new width and height are passed in so we make 
	//any necessary changes:
	Width = nw;
	Height = nh;
	//Tell the viewport to use all of our screen estate
	glViewport(0, 0, nw, nh);

	//This is not necessary, we're just working in 2d so
	//why not let our spaces reflect it?
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, static_cast<double>(Width)
		, 0.0, static_cast<double>(Height)
		, 1.0, -1.0);

	//Reserve memory for our render so that we don't do 
	//excessive allocations and render the image
	OutputImage.reserve(Width * Height * 3);
	render();
}


int main(int argc, char* argv[])
{
	// -------------------------------------------------
	// Initialize Window
	// -------------------------------------------------

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Width, Height, "Q1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//We have an opengl context now. Everything from here on out 
	//is just managing our window or opengl directly.

	//Tell the opengl state machine we don't want it to make 
	//any assumptions about how pixels are aligned in memory 
	//during transfers between host and device (like glDrawPixels(...) )
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//We call our resize function once to set everything up initially
	//after registering it as a callback with glfw
	glfwSetFramebufferSizeCallback(window, resize_callback);
	resize_callback(NULL, Width, Height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// -------------------------------------------------------------
		//Rendering begins!
		glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
		//and ends.
		// -------------------------------------------------------------

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//Close when the user hits 'q' or escape
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
