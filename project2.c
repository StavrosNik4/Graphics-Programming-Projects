#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stdbool.h>

float a = 4;
float b = 100;

float scaleFactor = 1.0f;      // current scaling factor
float scaleSpeed = 0.0005f;   // speed at which to scale the cube
bool increasing = true;      // whether to increase or decrease the scale factor

float rotate_angle = 0.0f; // initial rotation angle
const float rotate_axis[] = { 1.0f, 1.0f, 1.0f }; // rotation axis
const int square = 1;

#define PI 3.1415926535898

float angle = 0.0f;
float radius = 50.0f;
float objectX = 0.0f;
float objectY = 0.0f;

int question = 1;

void myinit(void) {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, 0.01, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glNewList(square, GL_COMPILE);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
    glEndList();

}


void drawCube() {
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -0.5f);// Apply translation to move the cube's center to the origin

    glTranslatef(0.0f, 0.0f, 0.5f);// Apply inverse translation to move the cube back to its original position
    // front face
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glCallList(square);
    glPopMatrix();

    // back face
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    glCallList(square);
    glPopMatrix();

    // left face
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // green
    glCallList(square);
    glPopMatrix();

    // right face
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 1.0f); // cyan
    glCallList(square);
    glPopMatrix();

    // top face
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glCallList(square);
    glPopMatrix();

    // bottom face
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 1.0f); // magenta
    glCallList(square);
    glPopMatrix();


    glPopMatrix();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // set up camera
    gluLookAt(0.0f, 0.0f, 0.0f,  // camera position
              0.0f, 0.0f, -1.0f, // point to look at
              0.0f, 1.0f, 0.0f); // up vector

    // Move the object along the circle
    objectX = radius * cos(angle);
    objectY = radius * sin(angle);

    // transformations

    if (question == 2) {    // B1
        glPushMatrix();
        glTranslatef(objectX, objectY, (-8.0) * b / 10.0);
        // rotate the cube
        glPushMatrix();
        glRotatef(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]); // Apply rotation transformations
    }
    else if (question == 1){    // A
        glPushMatrix();
        glTranslatef(0.0, 0.0, -b);
        // rotate the cube
        glPushMatrix();
        glRotatef(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]); // Apply rotation transformations
    }
    else {  // B2
        glPushMatrix();
        glTranslatef(0.0, 0.0, (-8.0) * b / 10.0);
        glPushMatrix();
        glRotatef(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]); // Apply rotation transformations
        glPushMatrix();
        glTranslatef(radius, -radius, 0.0);
    }

    // scale the cube
    glPushMatrix();
    glScalef(a * scaleFactor, a * scaleFactor, a * scaleFactor);

    // Draw the object
    drawCube();  // call your function to draw the cube

    
    if (question == 1 || question == 2) {
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }
    else {
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }

    glutSwapBuffers();// swap front and back buffers
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);

}

void idle() {

    // update the scaling factor
    if (increasing) {
        scaleFactor += scaleSpeed;
        if (scaleFactor >= 2.0f) {
            scaleFactor = 2.0f;
            increasing = false;
        }
    }
    else {
        scaleFactor -= scaleSpeed;
        if (scaleFactor <= 1.0f) {
            scaleFactor = 1.0f;
            increasing = true;
        }
    }

    rotate_angle += 0.03f; // continuously rotate the cube
    if (rotate_angle >= 360.0f) {
        // if rotation completes a full circle, start growing again
        rotate_angle = 0.0f;
    }

    // Increase the angle to move the object
    angle += 0.002f;
    if (angle >= 360.0f)
    {
        angle = 0.0f;
    }

    glutPostRedisplay();
}

// menu callback function
void menu(int id)
{

    if (id == 1) {
        question = 1;
        glutPostRedisplay();
    }

    if (id == 2) {
        question = 2;
        glutPostRedisplay();
    }

    if (id == 3) {
        question = 3;
        glutPostRedisplay();
    }
    
    if (id == 4) {
        exit(0);
    }
}

// create menu and options
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("A", 1);
    glutAddMenuEntry("B1", 2);
    glutAddMenuEntry("B2", 3);
    glutAddMenuEntry("Quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Square Display List with Color");

    myinit();
    createMenu(); // create menu and options

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // set up the idle function to be called every frame
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
