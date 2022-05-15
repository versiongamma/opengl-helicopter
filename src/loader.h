#pragma once
#include <stdio.h>
#include <freeglut.h>

#include "vecmath.h"
/******************************************************************************
 * Mesh Object Loader Setup and Prototypes
 ******************************************************************************/


typedef struct {
	int vertexIndex;	// Index of this vertex in the object's vertices array
	int texCoordIndex; // Index of the texture coordinate for this vertex in the object's texCoords array
	int normalIndex;	// Index of the normal for this vertex in the object's normals array
} meshObjectFacePoint;

typedef struct {
	int pointCount;
	meshObjectFacePoint* points;
} meshObjectFace;

typedef struct {
	int vertexCount;
	Vec3* vertices;
	int texCoordCount;
	Vec2* texCoords;
	int normalCount;
	Vec3* normals;
	int faceCount;
	meshObjectFace* faces;
} meshObject;

meshObject* loadMeshObject(char* fileName);
void renderMeshObject(meshObject* object);
void initMeshObjectFace(meshObjectFace* face, char* faceData, int faceDataLength);
void freeMeshObject(meshObject* object);