#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

# define M_PI  3.14159265358979323846

const int plain = 1;
const int roof_sides = 2;
const int roof = 3;
float angle = 0.0f;
float cameraX = 40.0;
float cameraZ = 20.0;
float cameraAngleX = 0.0f;
float cameraAngleY = 1.0f;
float cameraAngleZ = 0.0f;

void myInit() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-30.0, 30.0, -30.0, 30.0, -50.0, 150.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glNewList(plain, GL_COMPILE);
        glBegin(GL_POLYGON);
        glVertex3f(-100.0, 0.0, -100.0);
        glVertex3f(100.0, 0.0, -100.0);
        glVertex3f(100.0, 0.0, 100.0);
        glVertex3f(-100.0, 0.0, 100.0);
        glEnd();
    glEndList();

    glNewList(roof_sides, GL_COMPILE);
        glBegin(GL_TRIANGLES);
        glVertex3f(-10.0, 0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);

        glVertex3f(-10.0, 0.0, 20.0);
        glVertex3f(0.0, 10.0, 20.0);
        glVertex3f(10.0, 0.0, 20.0);
        glEnd();
   glEndList();

   glNewList(roof, GL_COMPILE);
       glBegin(GL_QUADS);
           glVertex3f(-5.0f, 0.0f, 10.0f);   // Top-left vertex
           glVertex3f(5.0f, 0.0f, 10.0f);    // Top-right vertex
           glVertex3f(5.0f, 0.0f, -10.0f);   // Bottom-right vertex
           glVertex3f(-5.0f, 0.0f, -10.0f);  // Bottom-left vertex
       glEnd();
   glEndList();
}
   
void drawHouse() {
    // Draw the main building (rectangular parallelepiped)
    glColor3f(0.329412, 0.329412, 0.329412); // arctic white color
    glPushMatrix();
    glScalef(10.0, 10.0, 20.0); // Dimensions: 10x10x20
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the triangular "roof"
    glColor3f(1.0 ,0.0 ,0.0); // red color
    glPushMatrix();
    glTranslatef(-4.0, 11.0, 0.0); // Position the roof above the main building
    glRotatef(-180.0, 90.0, 45.0, 0.0); // Rotate the roof to make it triangular
    glCallList(roof);
    glPopMatrix();

    // Draw the two triangles to "close" the triangular opening
    glColor3f(1.0 ,0.0 ,0.0); // red color
    glPushMatrix();
    glTranslatef(0.0, 5.0, -10.0); // Position the roof above the main building
    glCallList(roof_sides);
    glPopMatrix();

    glutPostRedisplay();
}

void drawGrass() {

    glColor3f(0.0, 0.5, 0.0); // dark Green color
    glPushMatrix();
    glTranslatef(0.0, -10.0, 0.0);
    glCallList(plain);
    glPopMatrix();
    
    glutPostRedisplay();
}

void moveCamera(int key, int x, int y) {
    const float rotationSpeed = 0.5f;  // Speed of rotation
    float radius = sqrt(pow(cameraX,2) + pow(cameraZ, 2));//find radius for rotation around center

    switch (key) {
    case GLUT_KEY_RIGHT:
        angle += rotationSpeed;  // Increase the angle for  rotation
        cameraX = radius * sin(angle * M_PI / 180.0);//angle * M_PI / 180.0 to convert angle into radians
        cameraZ = radius * cos(angle * M_PI / 180.0);
        break;
    case GLUT_KEY_LEFT:
        angle -= rotationSpeed;  // Decrease the angle for counterclockwise rotation
        cameraX = radius * sin(angle * M_PI / 180.0);
        cameraZ = radius * cos(angle * M_PI / 180.0);
        break;
    }
    
    glutPostRedisplay();  // Request a redraw of the scene
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Set the camera position
    gluLookAt(cameraX, 10.0, cameraZ,  // camera position
        0.0f, 0.0f, 0.0f, // point to look at
        0.0f, 1.0f, 0.0f); // up vector

    // Draw the house
    drawHouse();

    // Draw the grass
    drawGrass();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Project-3");

    myInit();

    glutSpecialFunc(moveCamera);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
