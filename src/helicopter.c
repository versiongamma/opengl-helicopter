#include "helicopter.h"

void helicopterMove(Helicopter* helicopter, TreeObject* trees, Vec3 velocity) {
	Vec3 rotatedVelocity = rotateVectorXZ(velocity, -helicopter->angle);
	helicopter->velocity = vec3Lerp(helicopter->velocity, rotatedVelocity, 0.15);

	Vec3 newPosition = (Vec3) {
		helicopter->position.x + helicopter->velocity.x,
		helicopter->position.y + helicopter->velocity.y,
		helicopter->position.z + helicopter->velocity.z
	};

	if (!helicopterCollision(helicopter, newPosition, trees)) { helicopter->position = newPosition; }
	else { helicopter->velocity = (Vec3) {0,0,0}; }
	
}

bool helicopterCollision(Helicopter* helicopter, Vec3 newPosition, TreeObject* trees) {
	// This is if exiting the bounds of the map
	if (sqrt(pow(newPosition.x, 2) + pow(newPosition.z, 2)) > 190) {
		helicopter->atEdge = TRUE;
		return TRUE;
	}

	if (newPosition.y < 0.55 || newPosition.y > 50) { return TRUE; }
	if (newPosition.y > 29) { return FALSE; }

	for (unsigned int i = 0; i < TREES_LENGTH; ++i) {
		GLfloat treeRadCol = 0;

		if (trees[i].modelIndex == 0) {
			treeRadCol = newPosition.y > 5 ? 11 : 6.5;
		}

		if (trees[i].modelIndex == 1) {
			treeRadCol = newPosition.y > 13 ? 11 : 6;
		}

		if (trees[i].modelIndex == 2) {
			treeRadCol = newPosition.y > 10 ? 9.5 : 4.5 ;
		}

		if (sqrt(
			pow(newPosition.x - trees[i].position.x, 2) + \
			pow(newPosition.z - trees[i].position.y, 2)) < treeRadCol) {
			return TRUE;
		} 
	}

	helicopter->atEdge = FALSE;
	return FALSE;
}

// Draws a cylinder with two spheres at the end for caps with given parameters
void drawCylinder(
	Helicopter* helicopter, 
	Vec3 offset, 
	GLfloat angle, 
	GLfloat radius, 
	GLfloat height, 
	GLUquadricObj* cylinderQuadric, 
	GLUquadricObj* sphereQuadric, 
	bool endCaps
) {
	glTranslatef(offset.x, offset.y, offset.z);
	glRotated(90 + angle, 0, 1, 0);
	glTranslated(0, 0, -height / 2.0f);
	gluCylinder(cylinderQuadric, radius, radius, height, 20, 20);

	if (endCaps) {
		gluSphere(sphereQuadric, radius, 20, 20);
		glRotated(-90, 0, 1, 0);
		glTranslated(height, 0, 0);
		gluSphere(sphereQuadric, radius, 20, 20);
		glRotated(-angle, 0, 1, 0);
	}
}

// Draws a flat cylinder for a rotor guard with given parameters
void drawRotorGuard(GLUquadricObj* cylinderQuadric) {
	setMaterial((RGB) { 51, 51, 51 }, (RGB) { 0, 0, 0 }, 80);
	glTranslatef(0, 0.12, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(cylinderQuadric, (HELI_ROTOR_LENGTH / 2) + 0.1, (HELI_ROTOR_LENGTH / 2) + 0.1, HELI_ROTOR_RADIUS * 2, 20, 20);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, -0.12, 0);
}

// Draws a low poly cylinder for a rotor with given parameters
void drawRotor(Helicopter* helicopter, Vec3 offset, GLfloat rotation, GLUquadricObj* cylinderQuadric) {
	setMaterial((RGB) { 18, 94, 161 }, (RGB) { 0, 0, 0 }, 80);
	glTranslatef(offset.x, offset.y, offset.z);
	glRotated(90, 0, 1, 0);
	
	glRotatef(rotation, 0, 1, 0);
	glTranslated(0, 0, -HELI_ROTOR_LENGTH / 2.0f);
	gluCylinder(cylinderQuadric, HELI_ROTOR_RADIUS, HELI_ROTOR_RADIUS, HELI_ROTOR_LENGTH, 4, 4);
	glTranslated(0, 0, HELI_ROTOR_LENGTH / 2.0f);
	glRotatef(-rotation - 90, 0, 1, 0);

	drawRotorGuard(cylinderQuadric);
}

void helicopterDisplay(Helicopter* helicopter, GLUquadricObj* cylinderQuadric, GLUquadricObj* sphereQuadric) {
	const double theta = -toRad(helicopter->angle);

	Vec3 cameraPosition = (Vec3) {
		helicopter->position.x - CAMERA_FOLLOW_DISTANCE * cos(theta),
		helicopter->position.y + CAMERA_HEIGHT_OFFSET,
		helicopter->position.z - CAMERA_FOLLOW_DISTANCE * sin(theta)
	};

	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z, helicopter->position.x, helicopter->position.y + 2, helicopter->position.z, 0, 1, 0);

	glPushMatrix();
	glTranslatef(helicopter->position.x, helicopter->position.y, helicopter->position.z);
	glRotatef(helicopter->angle, 0, 1, 0);

	const Vec3 pitch = rotateVectorXZ(helicopter->velocity, helicopter->angle - 90);
	glRotatef(vec3XZMagnitude(helicopter->velocity) * 30, pitch.x, 0, pitch.z);

	setMaterial((RGB) { 51, 51, 51 }, (RGB) { 0, 0, 0 }, 80);
	drawCylinder(helicopter, (Vec3) { 0, 0, 0 }, 0, HELI_BODY_RADIUS, HELI_BODY_LENGTH, cylinderQuadric, sphereQuadric, TRUE);
	drawCylinder(helicopter, (Vec3) { 0.3, 0, 1  }, 120, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric, TRUE);
	drawCylinder(helicopter, (Vec3) { 0.5, 0, -1.1  }, 60, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric, TRUE);
	drawCylinder(helicopter,(Vec3) { -4, 0, 0.8 }, 120, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric, TRUE);
	drawCylinder(helicopter, (Vec3) { 0.55, 0, 2.9 }, 60, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric, TRUE);

	drawRotor(helicopter, (Vec3) { -1, 0.2, -1.95 }, helicopter->rotorAngle, cylinderQuadric);
	drawRotor(helicopter, (Vec3) { 0, 0, 3.65 }, helicopter->rotorAngle, cylinderQuadric);
	drawRotor(helicopter, (Vec3) { 4.4, 0, -0.05 }, helicopter->rotorAngle, cylinderQuadric);
	drawRotor(helicopter, (Vec3) { 0, 0, -3.5 }, helicopter->rotorAngle, cylinderQuadric);
	

	// Drawing Lights
	setMaterial((RGB) { 51, 51, 51 }, (RGB) { 3, 0, 0 }, 80);
	drawCylinder(helicopter, (Vec3) { -3.7, 0.2, 1.72 }, 90, 0.08, 0.6, cylinderQuadric, sphereQuadric, FALSE);
	setMaterial((RGB) { 51, 51, 51 }, (RGB) { 0, 0, 0 }, 80);
	drawCylinder(helicopter, (Vec3) { -0.08, 0.05, 0.3 }, 90, 0.1, 0.6, cylinderQuadric, sphereQuadric, FALSE);
	drawCylinder(helicopter, (Vec3) { 2.6, 0, 0.3 }, 90, 0.08, 0.6, cylinderQuadric, sphereQuadric, FALSE);
	setMaterial((RGB) { 51, 51, 51 }, (RGB) { 2, 2, 2 }, 80);
	drawCylinder(helicopter, (Vec3) { -0.05, 0.02, 0.3 }, 90, 0.08, 0.6, cylinderQuadric, sphereQuadric, FALSE);

	// Reset the emission material
	setMaterial((RGB) { 0, 0, 0 }, (RGB) { 0, 0, 0 }, 80);

	glPopMatrix();

	if (helicopter->atEdge) {
		drawText("You cannot go any further!", (Vec2) { 20, 20 });
	}
}

void helicopterThink(Helicopter* helicopter, Quat4 controlQuaternion, TreeObject* trees, GLfloat DeltaTime) {
	const GLfloat distance = sqrt(pow(helicopter->position.x, 2) + pow(helicopter->position.z, 2));
	const GLfloat newFogDensityRange = ((distance * (0.036)) / 190) + 0.005;
	// This ludicrous power 6 polynomal function makes the fog only very slowly climb
	// as you reach the edges, and then ramp up sharply near the end, so most of the
	// time the fog is manageable, until we want it to completely block visibility.
	const GLfloat newFogDensity = 8800000 * pow(newFogDensityRange, 6) + 0.005;

	glFogf(GL_FOG_DENSITY, newFogDensity);
	if (helicopter->startup) {
		helicopter->rotorAngularVelocity += 8 ;
		helicopter->rotorAngle = angleClamp(helicopter->rotorAngle + helicopter->rotorAngularVelocity * DeltaTime);

		if (helicopter->rotorAngularVelocity >= ROTOR_SPEED) {
			helicopterMove(helicopter, trees, (Vec3) { 0, 0.3, 0 });

			if (helicopter->position.y >= 2) {
				helicopter->startup = FALSE;
			}
		}
		return;
	} 

	helicopter->angularVelocity = lerp(helicopter->angularVelocity, controlQuaternion.w * YAW_SPEED * DeltaTime, 0.15);
	helicopter->angle = angleClamp(helicopter->angle + helicopter->angularVelocity);
	helicopter->rotorAngle = angleClamp(helicopter->rotorAngle + ROTOR_SPEED * DeltaTime);

	helicopterMove(
		helicopter,
		trees,
		(Vec3) {
		controlQuaternion.x* MOVE_SPEED* DeltaTime,
			controlQuaternion.y* MOVE_SPEED* DeltaTime,
			controlQuaternion.z* MOVE_SPEED* DeltaTime
	}
	);
}
