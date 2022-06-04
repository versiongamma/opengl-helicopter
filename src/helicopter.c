#include "helicopter.h"

void helicopterMove(Helicopter* helicopter, Vec3 velocity) {
	Vec3 rotatedVelocity = rotateVectorXZ(velocity, -helicopter->angle);
	helicopter->velocity = vec3Lerp(helicopter->velocity, rotatedVelocity, 0.15);
	helicopter->position = (Vec3) { 
		helicopter->position.x + helicopter->velocity.x,
		helicopter->position.y + helicopter->velocity.y,
		helicopter->position.z + helicopter->velocity.z
	};
}

void drawPill(Helicopter* helicopter, Vec3 offset, GLfloat angle, GLfloat radius, GLfloat height, GLUquadricObj* cylinderQuadric, GLUquadricObj* sphereQuadric) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[4]) { 0, 0, 0, 0 });
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat[4]) { 0.2, 0.2, 0.2, 1 });
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat[4]) { 1, 1, 1, 1 });
	glMaterialf(GL_FRONT, GL_SHININESS, 80);

	glPushMatrix();
	glTranslatef(helicopter->position.x, helicopter->position.y, helicopter->position.z);
	glRotatef(helicopter->angle + angle, 0, 1, 0);
	glTranslatef(offset.x, offset.y, offset.z);
	glRotated(90, 0, 1, 0);
	glTranslated(0, 0, -height / 2.0f);
	gluCylinder(cylinderQuadric, radius, radius, height, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(helicopter->position.x, helicopter->position.y, helicopter->position.z);
	glRotatef(helicopter->angle + angle, 0, 1, 0);
	glTranslatef(offset.x, offset.y, offset.z);
	glTranslated(height / 2.0f, 0, 0);
	gluSphere(sphereQuadric, radius, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(helicopter->position.x, helicopter->position.y, helicopter->position.z);
	glRotatef(helicopter->angle + angle, 0, 1, 0);
	glTranslatef(offset.x, offset.y, offset.z);
	glTranslated(-height / 2.0f, 0, 0);
	gluSphere(sphereQuadric, radius, 20, 20);
	glPopMatrix();
}

void drawRotor(Helicopter* helicopter, Vec3 offset, GLfloat rotation, GLUquadricObj* cylinderQuadric) {
	glColor3f(0.1, 0.3, 0.9);

	glPushMatrix();
	glTranslatef(helicopter->position.x, helicopter->position.y, helicopter->position.z);
	glRotatef(helicopter->angle, 0, 1, 0);
	glTranslatef(offset.x, offset.y, offset.z);
	glRotatef(rotation, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	glRotated(45, 0, 0, 1);
	glTranslated(0, 0, -HELI_ROTOR_LENGTH / 2.0f);
	gluCylinder(cylinderQuadric, HELI_ROTOR_RADIUS, HELI_ROTOR_RADIUS, HELI_ROTOR_LENGTH, 4, 4);
	glPopMatrix();
}

void drawRotorGuard(Helicopter* helicopter, Vec3 offset, GLUquadricObj* cylinderQuadric) {
	glColor3f(0.2, 0.2, 0.2);

	glPushMatrix();
	glTranslatef(helicopter->position.x, helicopter->position.y, helicopter->position.z);
	glRotatef(helicopter->angle, 0, 1, 0);
	glTranslatef(offset.x, offset.y, offset.z);
	glRotatef(90, 1, 0, 0);
	gluCylinder(cylinderQuadric, HELI_ROTOR_LENGTH / 2, HELI_ROTOR_LENGTH / 2, HELI_ROTOR_RADIUS * 2, 20, 20);
	glPopMatrix();
}

void helicopterDisplay(Helicopter* helicopter, GLUquadricObj* cylinderQuadric, GLUquadricObj* sphereQuadric) {
	const double theta = -toRad(helicopter->angle);

	Vec3 cameraPosition = (Vec3) {
		helicopter->position.x - CAMERA_FOLLOW_DISTANCE * cos(theta),
		helicopter->position.y + CAMERA_HEIGHT_OFFSET,
		helicopter->position.z - CAMERA_FOLLOW_DISTANCE * sin(theta)
	};

	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z, helicopter->position.x, helicopter->position.y, helicopter->position.z, 0, 1, 0);
	
	drawPill(helicopter, (Vec3) { 0, 0, 0 }, 0, HELI_BODY_RADIUS, HELI_BODY_LENGTH, cylinderQuadric, sphereQuadric);
	drawPill(helicopter, (Vec3) { 2 , 0, -1  }, 120, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric);
	drawPill(helicopter, (Vec3) { -2 , 0, -1  }, 60, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric);
	drawPill(helicopter, (Vec3) { 2, 0, 1 }, 60, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric);
	drawPill(helicopter, (Vec3) { -2, 0, 1 }, 120, HELI_ARM_RADIUS, HELI_ARM_LENGTH, cylinderQuadric, sphereQuadric);

	drawRotor(helicopter, (Vec3) { 2.3, 0.3, -2 }, helicopter->rotorAngle, cylinderQuadric);
	drawRotor(helicopter, (Vec3) { -2.3, 0.3, -2 }, helicopter->rotorAngle, cylinderQuadric);
	drawRotor(helicopter, (Vec3) { -2.3, 0.3, 2 }, helicopter->rotorAngle, cylinderQuadric);
	drawRotor(helicopter, (Vec3) { 2.3, 0.3, 2 }, helicopter->rotorAngle, cylinderQuadric);

	drawRotorGuard(helicopter, (Vec3) { 2.3, 0.4, -2 }, cylinderQuadric);
	drawRotorGuard(helicopter, (Vec3) { -2.3, 0.4, -2 }, cylinderQuadric);
	drawRotorGuard(helicopter, (Vec3) { -2.3, 0.4, 2 }, cylinderQuadric);
	drawRotorGuard(helicopter, (Vec3) { 2.3, 0.4, 2 }, cylinderQuadric);
}

void helicopterThink(Helicopter* helicopter, Quat4 controlQuaternion, GLfloat DeltaTime) {
	helicopter->angularVelocity = lerp(helicopter->angularVelocity, controlQuaternion.w * YAW_SPEED * DeltaTime, 0.15);
	helicopter->angle = angleClamp(helicopter->angle + helicopter->angularVelocity);
	helicopter->rotorAngle = angleClamp(helicopter->rotorAngle + ROTOR_SPEED * DeltaTime);

	helicopterMove(
		helicopter, 
		(Vec3) { 
			controlQuaternion.x * MOVE_SPEED * DeltaTime,
			controlQuaternion.y * MOVE_SPEED * DeltaTime,
			controlQuaternion.z * MOVE_SPEED * DeltaTime
		}
	);
}
