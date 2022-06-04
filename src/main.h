#include <Windows.h>
#include <freeglut.h>
#include <math.h>
#include <stdio.h>

#include "helicopter.h"
#include "vecmath.h"
#include "loader.h"
#include "misc.h"

/******************************************************************************
 * Animation & Timing Setup
 ******************************************************************************/

 // Target frame rate (number of Frames Per Second).
#define TARGET_FPS 60	

/******************************************************************************
 * GLUT Callback Prototypes
 ******************************************************************************/

void display(void);
void reshape(int width, int height);
void idle(void);
void close(void);

/******************************************************************************
 * Animation-Specific Function Prototypes (add your own here)
 ******************************************************************************/

void main(int argc, char** argv);
void init(void);
void think(void);
void initLights(void);

void drawGround(void);
void drawSky(void);