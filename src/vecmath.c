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

GLfloat lerp(GLfloat a, GLfloat b, GLfloat t) {
	if (t > 1) return b;
	if (t < 0) return a;
	return (1 - t) * a + (b * t);
}

Vec3 vec3Lerp(Vec3 a, Vec3 b, GLfloat t) {
	if (t > 1) return b;
	if (t < 0) return a;

	return (Vec3) { lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t) };
}