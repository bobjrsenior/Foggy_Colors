#pragma once
#include "matrixMath.h"

void getMat4(mat4* matrix){
	matrix->x[0] = 1.0f;
	matrix->x[1] = 0.0f;
	matrix->x[2] = 0.0f;
	matrix->x[3] = 0.0f;
	matrix->y[0] = 0.0f;
	matrix->y[1] = 1.0f;
	matrix->y[2] = 0.0f;
	matrix->y[3] = 0.0f;
	matrix->z[0] = 0.0f;
	matrix->z[1] = 0.0f;
	matrix->z[2] = 1.0f;
	matrix->z[3] = 0.0f;
	matrix->w[0] = 0.0f;
	matrix->w[1] = 0.0f;
	matrix->w[2] = 0.0f;
	matrix->w[3] = 1.0f;
}

mat4* getNewMat4(){
	mat4* temp = (mat4*)malloc(sizeof(mat4));
	getMat4(temp);
	return temp;
}

void setVec4(vec4* vector, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w){
	vector->arr[0] = x;
	vector->arr[1] = y;
	vector->arr[2] = z;
	vector->arr[3] = w;
}

void copyVec4(vec4* vector, vec4* values){
	vector->arr[0] = values->arr[0];
	vector->arr[1] = values->arr[1];
	vector->arr[2] = values->arr[2];
	vector->arr[3] = values->arr[3];
}

void copyArrVec4(vec4* vector, const GLfloat* values){
	vector->arr[0] = values[0];
	vector->arr[1] = values[1];
	vector->arr[2] = values[2];
	vector->arr[3] = values[3];
}

vec4* makeNewVec4(const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w){
	vec4* temp = (vec4*)malloc(sizeof(vec4));;
	temp->arr[0] = x;
	temp->arr[1] = y;
	temp->arr[2] = z;
	temp->arr[3] = w;
	return temp;
}

void setVec3(vec3* vector, const GLfloat x, const GLfloat y, const GLfloat z){
	vector->arr[0] = x;
	vector->arr[1] = y;
	vector->arr[2] = z;
}

vec3* makeNewVec3(const GLfloat x, const GLfloat y, const GLfloat z){
	vec3* temp = (vec3*)malloc(sizeof(vec3));;
	temp->arr[0] = x;
	temp->arr[1] = y;
	temp->arr[2] = z;
	return temp;
}

void makeVec2(vec2* vector, const GLfloat x, const GLfloat y){
	vector->arr[0] = x;
	vector->arr[1] = y;
}

vec2* makeNewVec2(const GLfloat x, const GLfloat y){
	vec2* temp = (vec2*)malloc(sizeof(vec2));;
	temp->arr[0] = x;
	temp->arr[1] = y;
	return temp;
}

void scaleMat4(mat4* matrix, const vec3* amount){
	matrix->x[0] *= amount->arr[0];
	matrix->x[1] *= amount->arr[1];
	matrix->x[2] *= amount->arr[2];

	matrix->y[0] *= amount->arr[0];
	matrix->y[1] *= amount->arr[1];
	matrix->y[2] *= amount->arr[2];

	matrix->z[0] *= amount->arr[0];
	matrix->z[1] *= amount->arr[1];
	matrix->z[2] *= amount->arr[2];
}

mat4* scaleNewMat4(const vec3* amount){
	mat4* temp = getNewMat4();
	scaleMat4(temp, amount);
	return temp;
}

void translateMat4(mat4* matrix, vec3* amount){
	matrix->x[3] += amount->arr[0];
	matrix->y[3] += amount->arr[1];
	matrix->z[3] += amount->arr[2];
}

mat4* translateNewMat4(vec3* amount){
	mat4* temp = getNewMat4();
	translateMat4(temp, amount);
	return temp;
}

void rotateMat4(mat4* matrix, const GLfloat degrees, const vec3* direction){
	vec3* dir = normalizeNewVec3(direction);
	GLfloat sinDeg = degToRadSin(degrees);
	GLfloat cosDeg = degToRadCos(degrees);

	//http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
	//First row
	matrix->x[0] = dir->arr[0] + (cosDeg * (1.0 - (dir->arr[0] * dir->arr[0])));
	matrix->x[1] = ((dir->arr[0] * dir->arr[1]) * (1.0 - cosDeg)) - (dir->arr[2] * sinDeg);
	matrix->x[2] = ((dir->arr[0] * dir->arr[2]) * (1.0 - cosDeg)) + (dir->arr[1] * sinDeg);
	//Second row
	matrix->y[0] = ((dir->arr[0] * dir->arr[1]) * (1.0 - cosDeg)) + (dir->arr[2] * sinDeg);
	matrix->y[1] = (dir->arr[1] * dir->arr[1]) + ((1.0 - (dir->arr[1] * dir->arr[1])) * cosDeg);
	matrix->y[2] = ((dir->arr[1] * dir->arr[2]) * (1.0 - cosDeg)) - (dir->arr[0] * sinDeg);
	//Third row
	matrix->z[0] = ((dir->arr[0] * dir->arr[2]) * (1.0 - cosDeg)) - (dir->arr[1] * sinDeg);
	matrix->z[1] = ((dir->arr[1] * dir->arr[2]) * (1.0 - cosDeg)) + (dir->arr[0] * sinDeg);
	matrix->z[2] = (dir->arr[2] * dir->arr[2]) + (((1.0 - (dir->arr[2] * dir->arr[2]))) * cosDeg);
}

mat4* rotateNewMat4(const GLfloat degrees, const vec3* direction){
	mat4* temp = getNewMat4();
	rotateMat4(temp, degrees, direction);
	return temp;
}

void lookAtMat4(mat4* matrix, const vec3* camera, const vec3* target, const vec3* upVector){

	vec3* f = subNewVec3(target, camera);
	normalizeVec3(f);
	
	vec3* s = crossNewVec3(f, upVector);
	normalizeVec3(s);

	vec3* u = crossNewVec3(s, f);

	matrix->x[0] = s->arr[0];
	matrix->x[1] = u->arr[0];
	matrix->x[2] = -f->arr[0];
	matrix->x[3] = 0;

	matrix->y[0] = s->arr[1];
	matrix->y[1] = u->arr[1];
	matrix->y[2] = -f->arr[1];
	matrix->y[3] = 0;

	matrix->z[0] = s->arr[2];
	matrix->z[1] = u->arr[2];
	matrix->z[2] = -f->arr[2];
	matrix->z[3] = 0;

	matrix->w[0] = -dotVec3(s, camera);
	matrix->w[1] = -dotVec3(u, camera);
	matrix->w[2] = dotVec3(f, camera);
	matrix->w[3] = 1;

	free(f);
	free(u);
	free(s);
}

mat4* lookAtNewMat4(const vec3* camera, const vec3* target, const vec3* upVector){
	mat4* temp = (mat4*)malloc(sizeof(mat4));
	lookAtMat4(temp, camera, target, upVector);
	return temp;
}

void perspectiveMat4(mat4* matrix, const GLfloat fov, const GLfloat aspectRatio, const GLfloat nearClip, const GLfloat farClip){
	GLfloat tanHalfFov = degToRadTan(fov / 2.0f);

	matrix->x[0] = 1.0f / (aspectRatio * tanHalfFov);
	matrix->x[1] = 0;
	matrix->x[2] = 0;
	matrix->x[3] = 0;

	matrix->y[0] = 0;
	matrix->y[1] = 1.0f / (tanHalfFov);
	matrix->y[2] = 0;
	matrix->y[3] = 0;

	matrix->z[0] = 0;
	matrix->z[1] = 0;
	matrix->z[2] = -(farClip + nearClip) / (farClip - nearClip);
	matrix->z[3] = -1.0f;

	matrix->w[0] = 0;
	matrix->w[1] = 0;
	matrix->w[2] = -(2.0f * farClip * nearClip) / (farClip - nearClip);
	matrix->w[3] = 0;
}

mat4* perspectiveNewMat4(const GLfloat fov, const GLfloat aspectRatio, const GLfloat nearClip, const GLfloat farClip){
	mat4* temp = (mat4*)malloc(sizeof(mat4));
	perspectiveMat4(temp, fov, aspectRatio, nearClip, farClip);
	return temp;
}

GLfloat degToRadSin(const GLfloat degrees){
	return (GLfloat)sin(degrees * (M_PI / 180.0));
}

GLfloat degToRadCos(const GLfloat degrees){
	return (GLfloat)cos(degrees * (M_PI / 180.0));
}

GLfloat degToRadTan(const GLfloat degrees){
	return (GLfloat)tan(degrees * (M_PI / 180.0));
}

void normalizeVec3(vec3* vector){
	GLfloat* magnitude = (GLfloat*) malloc(sizeof(GLfloat));
	*magnitude = (vector->arr[0] * vector->arr[0]) + (vector->arr[1] * vector->arr[1]) + (vector->arr[2] * vector->arr[2]);
	*magnitude = sqrtf(*magnitude);
	vector->arr[0] /= *magnitude;
	vector->arr[1] /= *magnitude;
	vector->arr[2] /= *magnitude;
	free(magnitude);
}

vec3* normalizeNewVec3(const vec3* vector){
	vec3* temp = (vec3*)malloc(sizeof(vec3));
	temp->arr[0] = vector->arr[0];
	temp->arr[1] = vector->arr[1];
	temp->arr[2] = vector->arr[2];
	normalizeVec3(temp);
	return temp;
}

GLfloat magnitudeVec3(const vec3* vector){
	return sqrtf((vector->arr[0] * vector->arr[0]) + (vector->arr[1] * vector->arr[1]) + (vector->arr[2] * vector->arr[2]));
}

void subVec3(vec3* vector, const vec3* first, const vec3* second){
	vector->arr[0] = first->arr[0] - second->arr[0];
	vector->arr[1] = first->arr[1] - second->arr[1];
	vector->arr[2] = first->arr[2] - second->arr[2];
}

vec3* subNewVec3(const vec3* first, const vec3* second){
	vec3* temp = (vec3*)malloc(sizeof(vec3));
	subVec3(temp, first, second);
	return temp;
}

void addVec3(vec3* vector, const vec3* first, const vec3* second){
	vector->arr[0] = first->arr[0] + second->arr[0];
	vector->arr[1] = first->arr[1] + second->arr[1];
	vector->arr[2] = first->arr[2] + second->arr[2];
}

vec3* addNewVec3(const vec3* first, const vec3* second){
	vec3* temp = (vec3*)malloc(sizeof(vec3));
	addVec3(temp, first, second);
	return temp;
}

GLfloat dotVec3(const vec3* first, const vec3* second){
	return (first->arr[0] * second->arr[0]) + (first->arr[1] * second->arr[1]) + (first->arr[2] * second->arr[2]);
}

void crossVec3(vec3* vector, const vec3* first, const vec3* second){
	vector->arr[0] = (first->arr[1] * second->arr[2]) - (first->arr[2] * second->arr[1]);
	vector->arr[1] = (first->arr[2] * second->arr[0]) - (first->arr[0] * second->arr[2]);
	vector->arr[2] = (first->arr[0] * second->arr[1]) - (first->arr[1] * second->arr[0]);
}

vec3* crossNewVec3(const vec3* first, const vec3* second){
	vec3* temp = (vec3*)malloc(sizeof(vec3));
	crossVec3(temp, first, second);
	return temp;
}

void addMat4(mat4* matrix, const mat4* first, const mat4* second){
	matrix->x[0] = first->x[0] + second->x[0];
	matrix->x[1] = first->x[1] + second->x[1];
	matrix->x[2] = first->x[2] + second->x[2];
	matrix->x[3] = first->x[3] + second->x[3];

	matrix->y[0] = first->y[0] + second->y[0];
	matrix->y[1] = first->y[1] + second->y[1];
	matrix->y[2] = first->y[2] + second->y[2];
	matrix->y[3] = first->y[3] + second->y[3];

	matrix->z[0] = first->z[0] + second->z[0];
	matrix->z[1] = first->z[1] + second->z[1];
	matrix->z[2] = first->z[2] + second->z[2];
	matrix->z[3] = first->z[3] + second->z[3];

	matrix->w[0] = first->w[0] + second->w[0];
	matrix->w[1] = first->w[1] + second->w[1];
	matrix->w[2] = first->w[2] + second->w[2];
	matrix->w[3] = first->w[3] + second->w[3];
}

mat4* addNewMat4(const mat4* first, const mat4* second){
	mat4* temp = (mat4*)malloc(sizeof(mat4));
	addMat4(temp, first, second);
	return temp;
}

void multMat4(mat4* matrix, const mat4* first, const mat4* second){
	matrix->x[0] = (first->x[0] * second->x[0]) + (first->x[1] * second->y[0]) + (first->x[2] * second->z[0]) + (first->x[3] * second->w[0]);
	matrix->x[1] = (first->x[0] * second->x[1]) + (first->x[1] * second->y[1]) + (first->x[2] * second->z[1]) + (first->x[3] * second->w[1]);
	matrix->x[2] = (first->x[0] * second->x[2]) + (first->x[1] * second->y[2]) + (first->x[2] * second->z[2]) + (first->x[3] * second->w[2]);
	matrix->x[3] = (first->x[0] * second->x[3]) + (first->x[1] * second->y[3]) + (first->x[2] * second->z[3]) + (first->x[3] * second->w[3]);

	matrix->y[0] = (first->y[0] * second->x[0]) + (first->y[1] * second->y[0]) + (first->y[2] * second->z[0]) + (first->y[3] * second->w[0]);
	matrix->y[1] = (first->y[0] * second->x[1]) + (first->y[1] * second->y[1]) + (first->y[2] * second->z[1]) + (first->y[3] * second->w[1]);
	matrix->y[2] = (first->y[0] * second->x[2]) + (first->y[1] * second->y[2]) + (first->y[2] * second->z[2]) + (first->y[3] * second->w[2]);
	matrix->y[3] = (first->y[0] * second->x[3]) + (first->y[1] * second->y[3]) + (first->y[2] * second->z[3]) + (first->y[3] * second->w[3]);

	matrix->z[0] = (first->z[0] * second->x[0]) + (first->z[1] * second->y[0]) + (first->z[2] * second->z[0]) + (first->z[3] * second->w[0]);
	matrix->z[1] = (first->z[0] * second->x[1]) + (first->z[1] * second->y[1]) + (first->z[2] * second->z[1]) + (first->z[3] * second->w[1]);
	matrix->z[2] = (first->z[0] * second->x[2]) + (first->z[1] * second->y[2]) + (first->z[2] * second->z[2]) + (first->z[3] * second->w[2]);
	matrix->z[3] = (first->z[0] * second->x[3]) + (first->z[1] * second->y[3]) + (first->z[2] * second->z[3]) + (first->z[3] * second->w[3]);

	matrix->w[0] = (first->w[0] * second->x[0]) + (first->w[1] * second->y[0]) + (first->w[2] * second->z[0]) + (first->w[3] * second->w[0]);
	matrix->w[1] = (first->w[0] * second->x[1]) + (first->w[1] * second->y[1]) + (first->w[2] * second->z[1]) + (first->w[3] * second->w[1]);
	matrix->w[2] = (first->w[0] * second->x[2]) + (first->w[1] * second->y[2]) + (first->w[2] * second->z[2]) + (first->w[3] * second->w[2]);
	matrix->w[3] = (first->w[0] * second->x[3]) + (first->w[1] * second->y[3]) + (first->w[2] * second->z[3]) + (first->w[3] * second->w[3]);
}

mat4* multNewMat4(const mat4* first, const mat4* second){
	mat4* temp = malloc(sizeof(mat4));
	multMat4(temp, first, second);
	return temp;
}

void multMat4Vec3(vec3* vector, const mat4* first, const vec3* second){
	vector->arr[0] = (first->x[0] * second->arr[0]) + (first->x[1] * second->arr[1]) + (first->x[2] * second->arr[2]) + (first->x[3]);
	vector->arr[1] = (first->x[0] * second->arr[0]) + (first->x[1] * second->arr[1]) + (first->x[2] * second->arr[2]) + (first->x[3]);
	vector->arr[2] = (first->x[0] * second->arr[0]) + (first->x[1] * second->arr[1]) + (first->x[2] * second->arr[2]) + (first->x[3]);
}

vec3* multNewMat4Vec3(const mat4* first, const vec3* second){
	vec3* temp = (vec3*)malloc(sizeof(vec3));
	multMat4Vec3(temp, first, second);
	return temp;
}

void printMat4(const mat4* matrix){
	printf("%f %f %f %f\n", matrix->x[0], matrix->x[1], matrix->x[2], matrix->x[3]);
	printf("%f %f %f %f\n", matrix->y[0], matrix->y[1], matrix->y[2], matrix->y[3]);
	printf("%f %f %f %f\n", matrix->z[0], matrix->z[1], matrix->z[2], matrix->z[3]);
	printf("%f %f %f %f\n\n", matrix->w[0], matrix->w[1], matrix->w[2], matrix->w[3]);
}

void printVec4(const vec4* vector){
	printf("%f %f %f %f\n\n", vector->arr[0], vector->arr[1], vector->arr[2], vector->arr[3]);
}

void printVec3(const vec3* vector){
	printf("%f %f %f\n\n", vector->arr[0], vector->arr[1], vector->arr[2]);
}

void printVec2(const vec2* vector){
	printf("%f %f\n\n", vector->arr[0], vector->arr[1]);
}
