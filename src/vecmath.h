#pragma once
#include <freeglut.h>
#include <math.h>

typedef struct VECTOR3 {
	GLfloat x, y, z;
} Vec3;

typedef struct VECTOR2 {
	GLfloat x, y;
} Vec2;

// These quaternions won't be used in the way quaternions are normally used,
// these will just be used to carry rotation data on a single plane along position data
typedef struct QUATERNION {
	GLfloat x, y, z, w;
} Quat4;

// Converts degrees to radians
GLfloat toRad(GLfloat degree);
// Clamps an angle between 0 and 360 degrees
GLfloat angleClamp(GLfloat angle);
// Rotates a vector around the XZ plane
Vec3 rotateVectorXZ(Vec3 vector, GLfloat angle);
