#include "vecmath.h";

GLfloat toRad(GLfloat degree) { return degree * (3.141592653 / 180.0); }

GLfloat angleClamp(GLfloat angle) {
	if (angle < 0) { return 360 - angle; }
	if (angle > 360) { return -360 + angle; }
	return angle;
}

Vec3 rotateVectorXZ(Vec3 vector, GLfloat angle) {
	const double theta = toRad(angle);
	return (Vec3) { 
		vector.x * cos(theta) - vector.z * sin(theta), 
		vector.y, 
		vector.x * sin(theta) + vector.z * cos(theta) 
	}; 
}