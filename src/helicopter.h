#pragma once
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include "vecmath.h"

// Camera Parameters

#define CAMERA_FOLLOW_DISTANCE 10
#define CAMERA_HEIGHT_OFFSET 4

// Helicopter Speed Values (for movement calculations)

#define MOVE_SPEED 15
#define YAW_SPEED 90
#define ROTOR_SPEED 2000
#define ACCELERATION_ROTATION 0.2

// Helicopter Dimension Specifications

#define HELI_BODY_RADIUS 0.4
#define HELI_BODY_LENGTH 3
#define HELI_ARM_RADIUS 0.2
#define HELI_ARM_LENGTH 2
#define HELI_ROTOR_RADIUS 0.1
#define HELI_ROTOR_LENGTH 3.2

// Object for data storage of a helicopter's parameters
typedef struct HELICOPTER {
	Vec3 position; // The helicopter's current position in space
	Vec3 velocity; // The current velocity of the helicopter
	GLfloat angle; // The angle of the helicopter in the XZ plane in degrees
	GLfloat angularVelocity; // The current turn speed of the helicopter
	GLfloat rotorAngle; // The angle of the helicopter's rotors in the XZ plane in degrees
} Helicopter;

// Moves the helicopter based on a given position offset. This offset will be rotated along the XZ plane 
// according to the rotation of the helicopter, so can be given relative to the rotation of the helicopter
void helicopterMove(Helicopter* helicopter, Vec3 velocity);
// Function to draw a helictoper
void helicopterDisplay(Helicopter* helicopter, GLUquadricObj* cylinderQuadric, GLUquadricObj* sphereQuadric);
// Function to calculate a helicopter's updated parameters on a given frame
void helicopterThink(Helicopter* helicopter, Quat4 controlQuaternion, GLfloat DeltaTime);