#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "greg.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("What the dog doin? - Greg");

    glEnable(GL_DEPTH_TEST);

    setupProjection();

    // Set the display function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    createMenu();

    initQuadric();
    initializeDogParts();
    createTree();

    // Call this function after initializing matrices and before entering the GLUT main loop
    applyInitialTransformations();

    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}
