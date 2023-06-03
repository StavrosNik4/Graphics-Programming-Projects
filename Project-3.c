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
GLfloat light_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat light_position[] = { -50.0f, 0.0f, 0.0f, 1.0f }; // Will be updated
GLfloat mat_emission[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow emission color
// Global time variable
float time = 0.0f;

// The speed at which time progresses
float speed = 0.001f; // Adjust this value to your needs

typedef struct point3 {
    float x, y, z;
} point3;

point3 vertices[] = {
    {0.0, 0.0, 1.0},
    {0.0, 0.942809, -0.33333},
    {-0.816497, -0.471405, -0.333333},
    {0.816497, -0.471405, -0.333333}
};

void normalize(point3 *p) {
    double d = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
    p->x /= d;
    p->y /= d;
    p->z /= d;
}

point3 midpoint(const point3* a, const point3* b) {
    point3 result;
    result.x = (a->x + b->x) / 2.0;
    result.y = (a->y + b->y) / 2.0;
    result.z = (a->z + b->z) / 2.0;
    return result;
}

void divide_triangle(point3 a, point3 b, point3 c, int m) {
    if (m > 0) {
        point3 ab, ac, bc;

        // find midpoints and normalize
        ab = midpoint(&a, &b); normalize(&ab);
        ac = midpoint(&a, &c); normalize(&ac);
        bc = midpoint(&b, &c); normalize(&bc);

        // recurse
        divide_triangle(a, ab, ac, m - 1);
        divide_triangle(ab, b, bc, m - 1);
        divide_triangle(ac, bc, c, m - 1);
        divide_triangle(ab, bc, ac, m - 1);
    }
    else {
        // draw the triangle
        glBegin(GL_TRIANGLES);
        // Face 1
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();
    }
}

void myInit() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-30.0, 30.0, -30.0, 30.0, -190.0, 150.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glNewList(plain, GL_COMPILE);
        glBegin(GL_QUADS);
        glVertex3f(-50.0, 0.0, -50.0);
        glVertex3f(50.0, 0.0, -50.0);
        glVertex3f(50.0, 0.0, 50.0);
        glVertex3f(-50.0, 0.0, 50.0);
        glEnd();
    glEndList();

    glNewList(roof_sides, GL_COMPILE);
        glBegin(GL_TRIANGLES);
        glVertex3f(-5.0, 0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
        glVertex3f(5.0, 0.0, 0.0);
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
    glTranslatef(-2.5, 10.0, 0.0); // Position the roof above the main building
    glRotatef(-180.0,80.0, 50.0, 0.0); // Rotate the roof to make it triangular
    glCallList(roof);
    glPopMatrix();

    // Draw the triangular "roof"
    glColor3f(1.0, 0.0, 0.0); // red color
    glPushMatrix();
    glTranslatef(2.5, 10.0, 0.0); // Position the roof above the main building
    glRotatef(-180.0, -80.0, 50.0, 0.0); // Rotate the roof to make it triangular
    glCallList(roof);
    glPopMatrix();

    // Draw the two triangles to "close" the triangular opening
    glColor3f(1.0, 0.0, 0.0); // red color
    glPushMatrix();
    glTranslatef(0.0, 5.0, -10.0); // Position the roof above the main building
    glCallList(roof_sides);
    glPopMatrix();

    glColor3f(1.0 ,0.0 ,0.0); // red color
    glPushMatrix();
    glTranslatef(0.0, 5.0, 10.0); // Position the roof above the main building
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

void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f);
    
    glPushMatrix();

    glTranslatef(-50.0, 0.0, 0.0);

    // draw tetrahedron
    divide_triangle(vertices[0], vertices[1], vertices[2], 4);
    divide_triangle(vertices[3], vertices[2], vertices[1], 4);
    divide_triangle(vertices[0], vertices[3], vertices[1], 4);
    divide_triangle(vertices[0], vertices[2], vertices[3], 4);

    glPopMatrix();

    glutPostRedisplay();
}


void moveCamera(int key, int x, int y) {
    const float rotationSpeed = 1.0f;  // Speed of rotation
    float radius = sqrt(pow(cameraX,2) + pow(cameraZ, 2));//find radius for rotation around center

    switch (key) {
    case GLUT_KEY_RIGHT:
        angle += rotationSpeed;  // Increase the angle for clockwise rotation
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
    gluLookAt(cameraX, 15.0, cameraZ,  // camera position
        0.0f, 0.0f, 0.0f, // point to look at
        0.0f, 1.0f, 0.0f); // up vector

    // Draw the house
    drawHouse();

    // Draw the grass
    drawGrass();

    // Draw the sun
    drawSun();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void menu(int id)
{
    if (id == 4) {
        exit(0);
    }
}
void createMenu() {
    glutCreateMenu(menu);
   
    glutAddMenuEntry("Quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Project-3");

    myInit();
    createMenu(); // create menu and options

    glutSpecialFunc(moveCamera);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);


    glutMainLoop();

    return 0;
}
