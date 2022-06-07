#pragma once
#include "loader.h"
#include "misc.h"
#include "vecmath.h"

/*
 * <tree.c/tree.h> Defines the functions and datatypes required to generate and render
 * a tree in the scene
 */

typedef struct TREE {
	char* trunkFilePath;
	char* leavesFilePath;
	MeshObject* trunkObj;
	MeshObject* leavesObj;
} TreeModel;

typedef struct TreeObject {
	Vec2 position;
	GLuint modelIndex;
} TreeObject;

// Generates the data for the scene's trees
void generateTrees(TreeObject* trees);
// Draws a tree model from it's segment models
void treeDrawModelSegments(TreeModel* model);
// Frees all the models from a tree model
void treeClose(TreeModel* tree); 
// Draws all of the trees from a given array of trees
void treesDisplay(TreeObject* trees, GLuint* displayLists);
// For a given array of tree models, generates the display lists that draw each model.
// Returns an array of all of the list indexes
GLuint treeGenerateDisplayList(TreeModel* model);
