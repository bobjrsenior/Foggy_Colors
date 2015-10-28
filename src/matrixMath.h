//Includes

//Define GLfloat if it isn't already
#ifndef GLfloat
#define GLfloat float
#endif

#include <math.h>
//Allows for allocating memory
#include <stdlib.h>

#define M_PI 3.14159265358979323846

typedef struct mat4{
	GLfloat x[4];
	GLfloat y[4];
	GLfloat z[4];
	GLfloat w[4];
}mat4;

typedef struct vec4{
	GLfloat arr[4];
}vec4;

typedef struct vec3{
	GLfloat arr[3];
}vec3;

typedef struct vec2{
	GLfloat arr[2];
}vec2;

#define IS_VEC_4_EQUAL(x,y) (((x->arr[0]) == (y->arr[0])) && ((x->arr[1]) == (y->arr[1])) && ((x->arr[2]) == (y->arr[2])) && ((x->arr[3]) == (y->arr[3])))

void getMat4(mat4* matrix);

mat4* getNewMat4();

vec4* makeNewVec4(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);

void setVec4(vec4* vector, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);

void copyVec4(vec4* vector, vec4* values);

void copyArrVec4(vec4* vector, const GLfloat* values);

vec3* makeNewVec3(const GLfloat x, const GLfloat y, const GLfloat z);

void setVec3(vec3* vector, const GLfloat x, const GLfloat y, const GLfloat z);

vec2* makeNewVec2(const GLfloat x, const GLfloat y);

void setVec2(vec2* vector, const GLfloat x, const GLfloat y);

void scaleMat4(mat4* matrix, const vec3* amount);

mat4* scaleNewMat4(const vec3* amount);

void translateMat4(mat4* matrix, vec3* amount);

mat4* translateNewMat4(vec3* amount);

void rotateMat4(mat4* matrix, const GLfloat degrees, const vec3* direction);

mat4* rotateNewMat4(const GLfloat degrees, const vec3* direction);

void lookAtMat4(mat4* matrix, const vec3* camera, const vec3* target, const vec3* upVector);

mat4* lookAtNewMat4(const vec3* camera, const vec3* target, const vec3* upVector);

void perspectiveMat4(mat4* matrix, const GLfloat fov, const GLfloat aspectRatio, const GLfloat nearClip, const GLfloat farClip);

mat4* perspectiveNewMat4(const GLfloat fov, const GLfloat aspectRatio, const GLfloat nearClip, const GLfloat farClip);

GLfloat degToRadSin(const GLfloat degrees);

GLfloat degToRadCos(const GLfloat degrees);

GLfloat degToRadTan(const GLfloat degrees);

void normalizeVec3(vec3* vector);

vec3* normalizeNewVec3(const vec3* vector);

GLfloat magnitudeVec3(const vec3* vector);

vec3* subNewVec3(const vec3* first, const vec3* second);

void addVec3(vec3* vector, const vec3* first, const vec3* second);

vec3* addNewVec3(const vec3* first, const vec3* second);

GLfloat dotVec3(const vec3* first, const vec3* second);

void crossVec3(vec3* vector, const vec3* first, const vec3* second);

vec3* crossNewVec3(const vec3* first, const vec3* second);

void addMat4(mat4* matrix, const mat4* first, const mat4* second);

mat4* addNewMat4(const mat4* first, const mat4* second);

void multMat4(mat4* matrix, const mat4* first, const mat4* second);

mat4* multNewMat4(const mat4* first, const mat4* second);

void multMat4Vec3(vec3* vector, const mat4* first, const vec3* second);

vec3* multNewMat4Vec3(const mat4* first, const vec3* second);

void printMat4(const mat4* matrix);

void printVec4(const vec4* vector);

void printVec3(const vec3* matrix);

void printVec2(const vec2* vector);