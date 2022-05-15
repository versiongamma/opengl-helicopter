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

Helicopter helicopter;

MeshObject* loadedMesh;

/******************************************************************************
 * Entry Point (don't put anything except the main function here)
 ******************************************************************************/

void main(int argc, char **argv)
{
	// Initialize the OpenGL window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutCreateWindow("Helicopter goes brrrrrrrrrrrrrr");

	// Set up the scene.
	init();

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
void display(void)
{
	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix into the model view matrix
	glLoadIdentity();


	helicopterDisplay(&helicopter, cylinderQuadric, sphereQuadric);

	glColor3f(0, 1, 0);
	glTranslatef(10, 0, 0);
	renderMeshObject(loadedMesh);

	drawOrigin();
	drawGround();

	glutSwapBuffers();
}


void close(void) {
	freeMeshObject(loadedMesh);
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
void init(void)
{
	
	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// set background color to be black
	glClearColor(0, 0, 0, 1.0);

	
	initLights();

	sphereQuadric = gluNewQuadric();
	cylinderQuadric = gluNewQuadric();

	helicopter.position = (Vec3) { 0.0f, 1.0f, 0.0f };
	helicopter.angle = 0.0f;
	helicopter.rotorAngle = 0.0f;

	loadedMesh = loadMeshObject("assets/tree01.obj");
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

	helicopterThink(&helicopter, controlQuaternion, FRAME_TIME_SEC );
}

/*
	Initialise OpenGL lighting before we begin the render loop.

	Note (advanced): If you're using dynamic lighting (e.g. lights that move around, turn on or
	off, or change colour) you may want to replace this with a drawLights function that gets called
	at the beginning of display() instead of init().
*/
void initLights(void)
{
	// Simple lighting setup
	GLfloat globalAmbient[] = { 0.4f, 0.4f, 0.4f, 1 };
	GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat ambientLight[] = { 0, 0, 0, 1 };
	GLfloat diffuseLight[] = { 1, 1, 1, 1 };
	GLfloat specularLight[] = { 1, 1, 1, 1 };

	// Configure global ambient lighting.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Configure Light 0.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Make GL normalize the normal vectors we supply.
	glEnable(GL_NORMALIZE);

	// Enable use of simple GL colours as materials.
	glEnable(GL_COLOR_MATERIAL);
}

/******************************************************************************/


/*
   Draw an origin marker for modelling purposes
 */
void drawOrigin(void)
{
	glColor3f(0.0f, 1.0f, 1.0f);
	glutWireSphere(0.1, 10, 10);
	glBegin(GL_LINES);

	//x axis -red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	//y axis -green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);

	//z axis - blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 2.0f);

	glEnd();
}

void drawGround(void)
{
	glColor3d(0.1,0.5,0.2);

	glPushMatrix();

	glBegin(GL_QUADS);
	//back right corner
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(250, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(250, 0, -250);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, -250);
	//front right corner
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(250, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(250, 0, 250);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, 250);
	//front left corner
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-250, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-250, 0, 250);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, 250);
	//back left corner
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-250, 0, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-250, 0, -250);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 0, -250);

	glEnd();

	glPopMatrix();
}
