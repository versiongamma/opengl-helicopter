#pragma once
#include <freeglut.h>
#include <math.h>

// Object type for 3D Vector (x, y, z)
typedef struct VECTOR3 {
	GLfloat x, y, z;
} Vec3;

// Object type for 2D Vector (x, y)
typedef struct VECTOR2 {
	GLfloat x, y;
} Vec2;

// Object type for 4D Quaternion (x, y, z, w)
// These quaternions won't be used in the way quaternions are normally used,
// these will just be used to carry rotation data on a single plane along with position data
typedef struct QUATERNION {
	GLfloat x, y, z, w;
} Quat4;

// Converts given degrees to radians
GLfloat toRad(GLfloat degree);
// Clamps a given angle between 0 and 360 degrees
GLfloat angleClamp(GLfloat angle);
// Rotates a given vector around the XZ plane by the given angle in degrees
Vec3 rotateVectorXZ(Vec3 vector, GLfloat angle);
