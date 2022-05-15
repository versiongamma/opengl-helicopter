#pragma once
#include <stdio.h>
#include <freeglut.h>
#include "vecmath.h"


typedef struct {
	int vertexIndex;	// Index of this vertex in the object's vertices array
	int texCoordIndex; // Index of the texture coordinate for this vertex in the object's texCoords array
	int normalIndex;	// Index of the normal for this vertex in the object's normals array
} MeshObjectFacePoint;

typedef struct {
	int pointCount;
	MeshObjectFacePoint* points;
} MeshObjectFace;

typedef struct {
	int vertexCount;
	Vec3* vertices;
	int texCoordCount;
	Vec2* texCoords;
	int normalCount;
	Vec3* normals;
	int faceCount;
	MeshObjectFace* faces;
} MeshObject;

// Loads a Wavefront OBJ mesh file from a given file. Returns a pointer to the loaded mesh object
MeshObject* loadMeshObject(char* fileName);
// Renders a given mesh object
void renderMeshObject(MeshObject* object);
void initMeshObjectFace(MeshObjectFace* face, char* faceData, int faceDataLength);
void freeMeshObject(MeshObject* object);
// Load a binary ppm file into an OpenGL texture and return the OpenGL texture reference ID
int loadPPM(char* filename);