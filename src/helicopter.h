#pragma once
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include "vecmath.h"

#define CAMERA_FOLLOW_DISTANCE 10
#define CAMERA_HEIGHT_OFFSET 4

#define MOVE_SPEED 10
#define YAW_SPEED 60

// Helicopter Dimensions
#define HELI_BODY_RADIUS 0.4
#define HELI_BODY_LENGTH 3
#define HELI_ARM_RADIUS 0.2
#define HELI_ARM_LENGTH 2
#define HELI_ROTOR_RADIUS 0.1
#define HELI_ROTOR_LENGTH 3.2

#define ROTOR_SPEED 2000

typedef struct HELICOPTER {
	Vec3 position;
	GLfloat angle, rotorAngle;
} Helicopter;

// Moves the helicopter based on a given position offset. This offset will be rotated along the XZ plane 
// according to the rotation of the helicopter, so can be given relative to the rotation of the helicopter
void helicopterMove(Helicopter* helicopter, Vec3 newPositionOffset);
void helicopterDisplay(Helicopter* helicopter, GLUquadricObj* cylinderQuadric, GLUquadricObj* sphereQuadric, GLUquadricObj* diskQuadric);
void helicopterThink(Helicopter* helicopter, Quat4 controlQuaternion, GLfloat DeltaTime);