#include <GL/glut.h>
#include <stdbool.h>

float scaleFactor = 1.0f;    // current scaling factor
float scaleSpeed = 0.0005f;   // speed at which to scale the cube
bool increasing = true;      // whether to increase or decrease the scale factor

float rotate_angle = 0.0f; // initial rotation angle
const float rotate_axis[] = { 1.0f, 1.0f, 1.0f }; // rotation axis

void drawSquare(float r, float g, float b) {
    glColor3f(r, g, b); // set the color
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
}

void drawCube() {
    glScalef(4.0f, 4.0f, 4.0f); // scale the cube to be of size 4

    // front face
    glPushMatrix();
    drawSquare(1.0f, 0.0f, 0.0f); // red
    glPopMatrix();

    // back face
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    drawSquare(1.0f, 1.0f, 0.0f); // yellow
    glPopMatrix();

    // left face
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    drawSquare(0.0f, 1.0f, 0.0f); // green
    glPopMatrix();

    // right face
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawSquare(0.0f, 1.0f, 1.0f); // cyan
    glPopMatrix();

    // top face
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawSquare(0.0f, 0.0f, 1.0f); // blue
    glPopMatrix();

    // bottom face
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawSquare(1.0f, 0.0f, 1.0f); // magenta
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

    // scale the cube
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -100.0f);

    // rotate the cube
    glRotatef(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
   
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    drawCube();  // call your function to draw the cube

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
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

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Square Display List with Color");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // set up the idle function to be called every frame
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
