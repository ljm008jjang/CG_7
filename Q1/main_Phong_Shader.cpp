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
#include <fstream>
#include <sstream>

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
sphere_scene sphere(vec3(0, 1, 0), vec3(0, 0.5f, 0), vec3(0.5f, 0.5f, 0.5f), 32, vec3(0, 0, -7), 2);
// -------------------------------------------------

GLuint LoadShaders(const string& vertex_file_path, const string& fragment_file_path) {
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path.c_str(), ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossile to open %s", vertex_file_path.c_str());
		getchar();
		return 0;
	}

	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path.c_str(), ios::in);
	if (FragmentShaderStream.is_open()) {
		stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", vertex_file_path.c_str());
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling shader : %s\n", fragment_file_path.c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Update_mesh(const GLuint& VAO, const vector<GLuint>& GLBuffers, vector<vec3>& Positions,
	vector<vec3>& Normals, vector<vec3>& Colors, vector<unsigned int>& Indices) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(Positions[0]), &Positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Colors[0]), &Colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffers[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices[0]), &Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//helped by ChatGPT
void setUniformVec3(GLuint program, const std::string& name, const glm::vec3& value) {
	GLint loc = glGetUniformLocation(program, name.c_str());
	glUniform3fv(loc, 1, glm::value_ptr(value));
}
//helped by ChatGPT
void setUniformFloat(GLuint program, const std::string& name, float value) {
	GLint loc = glGetUniformLocation(program, name.c_str());
	glUniform1f(loc, value);
}
//helped by ChatGPT
void setUniformMat4(GLuint program, const std::string& name, const glm::mat4& mat) {
	GLint loc = glGetUniformLocation(program, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

//helped by ChatGPT
void render(GLuint shaderProgram, GLuint VAO, size_t indexCount)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -7));
	model = glm::scale(model, glm::vec3(2.0f));

	glm::vec3 eye = glm::vec3(0, 0, 0);
	glm::vec3 center = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::mat4 view = glm::lookAt(eye, center, up);

	glm::mat4 projection = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

	glm::mat4 mvp = projection * view * model;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

	// Set uniforms
	setUniformVec3(shaderProgram, "ka", vec3(0.0f, 1.0f, 0.0f));
	setUniformVec3(shaderProgram, "kd", vec3(0.0f, 0.5f, 0.0f));
	setUniformVec3(shaderProgram, "ks", vec3(0.5f));
	setUniformFloat(shaderProgram, "gamma", 2.2f);
	setUniformFloat(shaderProgram, "shininess", 32.0f);
	setUniformFloat(shaderProgram, "Ia", 0.2f);
	setUniformVec3(shaderProgram, "lightColor", vec3(1.0f));
	setUniformVec3(shaderProgram, "lightPos", vec3(-4.0f, 4.0f, -3.0f));
	setUniformVec3(shaderProgram, "viewPos", vec3(0.0f, 0.0f, 0.0f)); // 카메라 위치
	setUniformMat4(shaderProgram, "model", model);
	setUniformMat4(shaderProgram, "view", view);
	setUniformMat4(shaderProgram, "projection", projection);
	setUniformMat4(shaderProgram, "mvp", mvp);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void resize_callback(GLFWwindow* window, int nw, int nh)
{
	Width = nw;
	Height = nh;
	glViewport(0, 0, Width, Height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


int main(int argc, char* argv[])
{
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint shaderProgram = LoadShaders("Phong.vert", "Phong.frag");

	sphere.clear();
	sphere.create_scene();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	vector<GLuint> GLBuffers(4, 0);
	glGenBuffers(4, &GLBuffers[0]);
	Update_mesh(VAO, GLBuffers, sphere.gVertexBuffer, sphere.gNormalBuffer, sphere.gColorBuffer, sphere.gIndexBuffer);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		render(shaderProgram, VAO, sphere.gIndexBuffer.size());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}