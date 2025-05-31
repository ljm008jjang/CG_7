//
//  sphere_scene.c
//  Rasterizer
//
//
#pragma once
#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#ifndef M_PI  
#define M_PI 3.14159265358979323846  
#endif
using namespace glm;
using namespace std;

class sphere_scene
{
public:
    sphere_scene(vec3 Ka, vec3 Kd, vec3 Ks, int p, vec3 position, float scale) : Ka(Ka), Kd(Kd), Ks(Ks), p(p), position(position), scale(scale) {}

    int     gNumVertices = 0;    // Number of 3D vertices.
    int     gNumTriangles = 0;    // Number of triangles.
    vector<unsigned int> gIndexBuffer; // Vertex indices for the triangles.

    vector<vec3> gVertexBuffer; // Vertex location for the triangles.
    vector<vec3> gNormalBuffer; // Vertex location for the triangles.
    vector<vec3> gColorBuffer; // Vertex location for the triangles.

    vec3 Ka, Kd, Ks;
    int p;

    vec3 position;
    float scale;

    void clear() {
	    gNumVertices = 0;
	    gNumTriangles = 0;
	    gIndexBuffer.clear();
	    gVertexBuffer.clear();
	    gNormalBuffer.clear();
		gColorBuffer.clear();
	}


    void create_scene()
    {
        int width = 32;
        int height = 16;

        float theta, phi;
        int t;

        gNumVertices = (height - 2) * width + 2;
        gNumTriangles = (height - 2) * (width - 1) * 2;

        // TODO: Allocate an array for gNumVertices vertices.
        gVertexBuffer.resize(gNumVertices);
		gNormalBuffer.resize(gNumVertices);
		gColorBuffer.resize(gNumVertices, vec3(1,1,1));
        gIndexBuffer.resize(3 * gNumTriangles);

        t = 0;
        for (int j = 1; j < height - 1; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                theta = (float)j / (height - 1) * M_PI;
                phi = (float)i / (width - 1) * M_PI * 2;

                float   x = sinf(theta) * cosf(phi);
                float   y = cosf(theta);
                float   z = -sinf(theta) * sinf(phi);

                // TODO: Set vertex t in the vertex array to {x, y, z}.
                gVertexBuffer[t].x = x;
                gVertexBuffer[t].y = y;
                gVertexBuffer[t].z = z;
                gNormalBuffer[t] = normalize(vec3(gVertexBuffer[t]));
                t++;
            }
        }

        // TODO: Set vertex t in the vertex array to {0, 1, 0}.
        gVertexBuffer[t].x = 0;
        gVertexBuffer[t].y = 1;
        gVertexBuffer[t].z = 0;
        gNormalBuffer[t] = normalize(vec3(gVertexBuffer[t]));
        t++;

        // TODO: Set vertex t in the vertex array to {0, -1, 0}.
        gVertexBuffer[t].x = 0;
        gVertexBuffer[t].y = -1;
        gVertexBuffer[t].z = 0;
        gNormalBuffer[t] = normalize(vec3(gVertexBuffer[t]));
        t++;

        t = 0;
        for (int j = 0; j < height - 3; ++j)
        {
            for (int i = 0; i < width - 1; ++i)
            {
                gIndexBuffer[t++] = j * width + i;
                gIndexBuffer[t++] = (j + 1) * width + (i + 1);
                gIndexBuffer[t++] = j * width + (i + 1);
                gIndexBuffer[t++] = j * width + i;
                gIndexBuffer[t++] = (j + 1) * width + i;
                gIndexBuffer[t++] = (j + 1) * width + (i + 1);
            }
        }
        for (int i = 0; i < width - 1; ++i)
        {
            gIndexBuffer[t++] = (height - 2) * width;
            gIndexBuffer[t++] = i;
            gIndexBuffer[t++] = i + 1;
            gIndexBuffer[t++] = (height - 2) * width + 1;
            gIndexBuffer[t++] = (height - 3) * width + (i + 1);
            gIndexBuffer[t++] = (height - 3) * width + i;
        }

        // The index buffer has now been generated. Here's how to use to determine
        // the vertices of a triangle. Suppose you want to determine the vertices
        // of triangle i, with 0 <= i < gNumTriangles. Define:
        //
        // k0 = gIndexBuffer[3*i + 0]
        // k1 = gIndexBuffer[3*i + 1]
        // k2 = gIndexBuffer[3*i + 2]
        //
        // Now, the vertices of triangle i are at positions k0, k1, and k2 (in that
        // order) in the vertex array (which you should allocate yourself at line
        // 27).
        //
        // Note that this assumes 0-based indexing of arrays (as used in C/C++,
        // Java, etc.) If your language uses 1-based indexing, you will have to
        // add 1 to k0, k1, and k2.
    }

 //   void process_triangle(mat4 MVP,mat4 model, int Width, int Height,vec3* v0, vec3* v1, vec3* v2, vec3* screen0, vec3* screen1, vec3* screen2, float*invW0, float* invW1, float* invW2, int index) {

 //       int k0 = gIndexBuffer[3 * index + 0];
 //       int k1 = gIndexBuffer[3 * index + 1];
 //       int k2 = gIndexBuffer[3 * index + 2];

 //       vec3 p0 = vec3(gVertexBuffer[3 * k0 + 0], gVertexBuffer[3 * k0 + 1], gVertexBuffer[3 * k0 + 2]);
 //       vec3 p1 = vec3(gVertexBuffer[3 * k1 + 0], gVertexBuffer[3 * k1 + 1], gVertexBuffer[3 * k1 + 2]);
 //       vec3 p2 = vec3(gVertexBuffer[3 * k2 + 0], gVertexBuffer[3 * k2 + 1], gVertexBuffer[3 * k2 + 2]);

 //       // 삼각형 점 좌표
 //       *v0 = vec3(model * vec4(p0, 1.0f));
 //       *v1 = vec3(model * vec4(p1, 1.0f));
 //       *v2 = vec3(model * vec4(p2, 1.0f));

 //       vec4 clip0 = MVP * vec4(p0, 1.0f);
 //       vec4 clip1 = MVP * vec4(p1, 1.0f);
 //       vec4 clip2 = MVP * vec4(p2, 1.0f);

 //       // 이건 clip space 결과에서 바로 계산해야 함
 //       *invW0 = 1.0f / clip0.w;
 //       *invW1 = 1.0f / clip1.w;
 //       *invW2 = 1.0f / clip2.w;

 //       // Viewport Transform 계산
 //       mat4 vp(0.0f);
 //       vp[0][0] = Width / 2.0f;
 //       vp[1][1] = Height / 2.0f;
 //       vp[2][2] = 1.0f;
 //       vp[3][0] = (Width - 1) / 2.0f;
 //       vp[3][1] = (Height - 1) / 2.0f;
 //       vp[3][3] = 1.0f;

 //       // 화면 좌표
 //       *screen0 = vec3(vp * (clip0 / clip0.w));
 //       *screen1 = vec3(vp * (clip1 / clip1.w));
 //       *screen2 = vec3(vp * (clip2 / clip2.w));
 //   }

	//mat4 get_model_transform()
	//{
	//	mat4 model = translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), vec3(scale));
	//	return model;
	//}
    
};
