/******************************************************************************
 *
 * Computer Graphics Programming 2020 Project Template v1.0 (11/04/2021)
 *
 * Based on: Animation Controller v1.0 (11/04/2021)
 *
 * This template provides a basic FPS-limited render loop for an animated scene,
 * plus keyboard handling for smooth game-like control of an object such as a
 * character or vehicle.
 *
 * A simple static lighting setup is provided via initLights(), which is not
 * included in the animationalcontrol.c template. There are no other changes.
 *
 ******************************************************************************/

#include "main.h"

// Ideal time each frame should be displayed for (in milliseconds).
const unsigned int FRAME_TIME = 1000 / TARGET_FPS;
// Frame time in fractional seconds.
// Note: This is calculated to accurately reflect the truncated integer value of
// FRAME_TIME, which is used for timing, rather than the more accurate fractional
// value we'd get if we simply calculated "FRAME_TIME_SEC = 1.0f / TARGET_FPS".
const float FRAME_TIME_SEC = (1000 / TARGET_FPS) / 1000.0f;
// Time we started preparing the current frame (in milliseconds since GLUT was initialized).
unsigned int frameStartTime = 0;


// pointer to quadric objects
GLUquadricObj* sphereQuadric;
GLUquadricObj* cylinderQuadric;

// Animated object parameters
Helicopter helicopter;
GLfloat waterHeight, waterOffset;

// Meshes and Textures
MeshObject* pondModel;
TreeModel treeModel01, treeModel02, treeModel03;
GLuint treeDisplayList01, treeDisplayList02, treeDisplayList03;
TreeObject trees[TREES_LENGTH];
GLuint groundTexture, skyTexture, waterTexture;

/******************************************************************************
 * Entry Point (don't put anything except the main function here)
 ******************************************************************************/

void main(int argc, char **argv) {
	bool fullBright = FALSE;
	if (argc > 1) {
		fullBright = !strcmp(argv[1], "--fullbright");
	}

	// Initialize the OpenGL window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutCreateWindow("OpenGL Drone | 19076935");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Because it's taking a little over a second or two to load,
	// I want to display something while it loads
	drawText("Loading...", (Vec2) {DEFAULT_WINDOW_WIDTH / 2 - 45, DEFAULT_WINDOW_HEIGHT /2 });
	glutSwapBuffers();

	// Set up the scene.
	init();
	initLights(fullBright);

	// Disable key repeat (keyPressed or specialKeyPressed will only be called once when a key is first pressed).
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Register GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutSpecialUpFunc(specialKeyReleased);
	glutIdleFunc(idle);
	glutCloseFunc(close);

	// Record when we started rendering the very first frame (which should happen after we call glutMainLoop).
	frameStartTime = (unsigned int)glutGet(GLUT_ELAPSED_TIME);

	// Enter the main drawing loop (this will never return).
	glutMainLoop();
}

/******************************************************************************
 * GLUT Callbacks (don't add any other functions here)
 ******************************************************************************/

 /*
	 Called when GLUT wants us to (re)draw the current animation frame.

	 Note: This function must not do anything to update the state of our simulated
	 world. Animation (moving or rotating things, responding to keyboard input,
	 etc.) should only be performed within the think() function provided below.
 */
void display(void) {
	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load the identity matrix into the model view matrix
	glLoadIdentity();

	helicopterDisplay(&helicopter, cylinderQuadric, sphereQuadric);
	
	drawGround();
	drawWater();
	drawSky();

	treesDisplay(trees, (GLuint[3]) { treeDisplayList01, treeDisplayList02, treeDisplayList03 });
	glutSwapBuffers();
}


void close(void) {
	treeClose(&treeModel01);
	treeClose(&treeModel02);
	treeClose(&treeModel03);
	freeMeshObject(pondModel);
}

/*
	Called by GLUT when it's not rendering a frame.

	Note: We use this to handle animation and timing. You shouldn't need to modify
	this callback at all. Instead, place your animation logic (e.g. moving or rotating
	things) within the think() method provided with this template.
*/
void idle(void)
{
	// Wait until it's time to render the next frame.

	unsigned int frameTimeElapsed = (unsigned int)glutGet(GLUT_ELAPSED_TIME) - frameStartTime;
	if (frameTimeElapsed < FRAME_TIME)
	{
		// This frame took less time to render than the ideal FRAME_TIME: we'll suspend this thread for the remaining time,
		// so we're not taking up the CPU until we need to render another frame.
		unsigned int timeLeft = FRAME_TIME - frameTimeElapsed;
		Sleep(timeLeft);
	}

	// Begin processing the next frame.

	frameStartTime = glutGet(GLUT_ELAPSED_TIME); // Record when we started work on the new frame.

	think(); // Update our simulated world before the next call to display().

	glutPostRedisplay(); // Tell OpenGL there's a new frame ready to be drawn.
}

/******************************************************************************
 * Animation-Specific Functions (Add your own functions at the end of this section)
 ******************************************************************************/

 /*
	 Initialise OpenGL and set up our scene before we begin the render loop.
 */
void init(void) {

	srand(time(NULL));
	
	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	sphereQuadric = gluNewQuadric();
	cylinderQuadric = gluNewQuadric();

	helicopter.position = (Vec3) { -100.0f, 1.0f, 0.0f };
	helicopter.velocity = (Vec3) { 0, 0, 0 };
	helicopter.angle = 0.0f;
	helicopter.rotorAngle = 0.0f;
	helicopter.rotorAngularVelocity = 0.0f;
	helicopter.startup = TRUE;

	waterHeight = -1;
	waterOffset = -50;
	
	pondModel = loadMeshObject("plane.obj");
	treeLoad(&treeModel01, "tree01trunk.obj", "tree01leaves.obj");
	treeLoad(&treeModel02, "tree02trunk.obj", "tree02leaves.obj");
	treeLoad(&treeModel03, "tree03trunk.obj", "tree03leaves.obj");

	groundTexture = loadPPM("ground_color.PPM");
	skyTexture = loadPPM("sky_color.ppm");
	waterTexture = loadPPM("water_color.ppm");

	generateTrees(trees);
	treeDisplayList01 = treeGenerateDisplayList(&treeModel01);
	treeDisplayList02 = treeGenerateDisplayList(&treeModel02);
	treeDisplayList03 = treeGenerateDisplayList(&treeModel03);

}

/*
	Advance our animation by FRAME_TIME milliseconds.

	Note: Our template's GLUT idle() callback calls this once before each new
	frame is drawn, EXCEPT the very first frame drawn after our application
	starts. Any setup required before the first frame is drawn should be placed
	in init().
*/
void think(void) {

	/*
		Keyboard motion handler: complete this section to make your "player-controlled"
		object respond to keyboard input.
	*/

	Quat4 controlQuaternion = { 0, 0, 0, 0 };

	if (getKeyboardState().Yaw != MOTION_NONE) {
		controlQuaternion.w = getKeyboardState().Yaw;
	}
	if (getKeyboardState().Surge != MOTION_NONE) {
		controlQuaternion.x = getKeyboardState().Surge;
	}
	if (getKeyboardState().Sway != MOTION_NONE) {
		controlQuaternion.z = getKeyboardState().Sway;
	}
	if (getKeyboardState().Heave != MOTION_NONE) {
		controlQuaternion.y = getKeyboardState().Heave;
	}

	helicopterThink(&helicopter, controlQuaternion, trees, FRAME_TIME_SEC );
	const GLfloat x = FRAME_TIME_SEC * (GLfloat)frameStartTime / 10;
	waterHeight = (sin(x) / 8) - 1;
	waterOffset = waterOffset >= 50 ? -50 : waterOffset + 1 * FRAME_TIME_SEC;

}

/*
	Initialise OpenGL lighting before we begin the render loop.

	Note (advanced): If you're using dynamic lighting (e.g. lights that move around, turn on or
	off, or change colour) you may want to replace this with a drawLights function that gets called
	at the beginning of display() instead of init().
*/
void initLights(bool fullBright) {

	GLfloat light0position[] = { 0, 2, -13, 1 };
	GLfloat light0diffuse[] = { 3.5, 3.5, 3.5, 1 };
	GLfloat light0specular[] = { 0.2, 0.2, 0.2, 1 };

	GLfloat light1position[] = { 0, 2, -2, 1.0 };
	GLfloat light1diffuse[] = { 15, 0, 0, 1.0 };
	GLfloat light1specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0position);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1position);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 2);


	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	if (fullBright) {
		glLightfv(GL_LIGHT2, GL_DIFFUSE, (GLfloat[4]) { 1, 1, 1, 1 });
		glLightfv(GL_LIGHT2, GL_SPECULAR, light1specular);
		glLightfv(GL_LIGHT2, GL_POSITION, (GLfloat[4]) { 0, 100, 0, 1 });
		glEnable(GL_LIGHT2);
	}

	GLfloat fogColor[4] = { 0,0,0,1 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_MODE, GL_EXP);

	glEnable(GL_FOG);

	// Make GL normalize the normal vectors we supply.
	glEnable(GL_NORMALIZE);

}

void drawWater(void) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[4]) { 0, 0, 0, 0 });
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat[4]) { 0.2, 0.4, 1, 0.8 });
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat[4]) { 1, 1, 1, 1 });
	glMaterialf(GL_FRONT, GL_SHININESS, 80);

	const GLfloat spacing = 5;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, waterTexture);

	glPushMatrix();
	glRotatef(30, 0, 1, 0);
	glTranslatef(waterOffset, 0, 0);
	glBegin(GL_QUADS);
	for (GLfloat x = -100; x <= 100; x += spacing) {
		for (GLfloat z = -100; z <= 100; z += spacing) {

			glVertex3d(x, waterHeight, z);
			glNormal3d(0, 1, 0);
			glTexCoord2f(0, 0);


			glVertex3d(x + spacing, waterHeight, z);
			glNormal3d(0, 1, 0);
			glTexCoord2f(1, 0);


			glVertex3d(x + spacing, waterHeight, z + spacing);
			glNormal3d(0, 1, 0);
			glTexCoord2f(1, 1);


			glVertex3d(x, waterHeight, z + spacing);
			glNormal3d(0, 1, 0);
			glTexCoord2f(0, 1);

		}
	}
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

/****************************************************************************/
void drawGround(void) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[4]) { 0, 0, 0, 0 });
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat[4]) { 1, 1, 1, 1 });
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat[4]) { 1, 1, 1, 1 });
	glMaterialf(GL_FRONT, GL_SHININESS, 5);

	const GLfloat spacing = 5;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, groundTexture);

	glPushMatrix();
	renderMeshObject(pondModel);
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	for (GLfloat x = -250; x <= 250; x += spacing) {
		for (GLfloat z = -250; z <= 250; z += spacing) {
			if (x < 40 && x > -40 && z < 40 && z > -40) {
				continue;
			}

			glVertex3d(x, 0, z);
			glNormal3d(0, 1, 0);
			glTexCoord2f(0, 0);


			glVertex3d(x + spacing, 0, z);
			glNormal3d(0, 1, 0);
			glTexCoord2f(1, 0);


			glVertex3d(x + spacing, 0, z + spacing);
			glNormal3d(0, 1, 0);
			glTexCoord2f(1, 1);


			glVertex3d(x, 0, z + spacing);
			glNormal3d(0, 1, 0);
			glTexCoord2f(0, 1);
		}
	}

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawSky(void) {
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, skyTexture);

	glMaterialfv(GL_BACK, GL_EMISSION, (GLfloat[4]) { 1, 1, 1, 1 });
	glMaterialfv(GL_BACK, GL_DIFFUSE, (GLfloat[4]) { 1, 1, 1, 1 });
	glMaterialfv(GL_BACK, GL_SPECULAR, (GLfloat[4]) { 1, 1, 1, 1 });
	gluQuadricTexture(sphereQuadric, TRUE);

	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	gluSphere(sphereQuadric, 250, 20, 20);
	glPopMatrix();

	gluQuadricTexture(sphereQuadric, FALSE);
	glDisable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}
