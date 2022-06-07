#define _CRT_SECURE_NO_WARNINGS
#include "tree.h"

void treeLoad(TreeModel* model, char* trunkFilePath, char* leavesFilePath) {
	model->trunkFilePath = trunkFilePath;
	model->leavesFilePath = leavesFilePath;
	model->trunkObj = loadMeshObject(trunkFilePath);
	model->leavesObj = loadMeshObject(leavesFilePath);
}

void treeDrawModelSegments(TreeModel* model) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[4]) { 0, 0, 0, 0 });
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat[4]) { 74.f / 255.f, 37.f / 255.f, 14.f / 255.f, 1 });
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat[4]) { 0, 0, 0, 1 });
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
	renderMeshObject(model->trunkObj);

	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[4]) { 0, 0, 0, 0 });
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat[4]) { 22.f / 255.f, 61.f / 255.f, 7.f / 255.f, 1 });
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat[4]) { 0, 0, 0, 1 });
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
	renderMeshObject(model->leavesObj);
}

void treeClose(TreeModel* tree) {
	freeMeshObject(tree->trunkObj);
	freeMeshObject(tree->leavesObj);
}

void generateTrees(TreeObject* trees) {
	GLfloat treeValues[TREES_LENGTH][3];
	loadTrees(&treeValues);

	for (unsigned int i = 0; i < TREES_LENGTH; ++i) {
		trees[i].position = (Vec2){ treeValues[i][0], treeValues[i][1] };
		trees[i].modelIndex = (GLuint)treeValues[i][2];
	}

}

void treesDisplay(TreeObject* trees, GLuint* displayLists) {
	for (unsigned int i = 0; i < TREES_LENGTH; ++i) {
		glPushMatrix();
		glTranslatef(trees[i].position.x, 0, trees[i].position.y);
		glCallList(displayLists[trees[i].modelIndex]);
		glPopMatrix();
	}
}


GLuint treeGenerateDisplayList(TreeModel* model) {
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	treeDrawModelSegments(model);
	glEndList();

	return index;
}