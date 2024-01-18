#include <stdio.h>
#include <GL/glut.h>
#include <math.h>


# define M_PI  3.14159265358979323846

// Initial sphere position
float sphere_x = -50.0, sphere_y = 0.0, sphere_z = 0.0;
float shpere_angle = 0.0f;
float intensity = 0.0f;

const int plain = 1;
const int roof_sides = 2;
const int roof_sides_2 = 5;
const int roof = 3;
const int main_build = 4;
float angle = 0.0f;
float cameraX = 40.0;
float cameraZ = 40.0;
GLfloat light_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // 0.3 .3 .3
GLfloat light_position[] = { -50.0f, 0.0f, 0.0f, 1.0f }; // Will be updated
GLfloat mat_emission[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow emission color

int polygon = 1;
int smooth = 1;
int spotlight_trigger = 1;

float A[3] = { 0.0f, 0.0f, 0.0f };
float B[3] = { 0.0f, 0.0f, 0.0f };

float VertexA[3] = { 0.0f, 0.0f, 0.0f };
float VertexB[3] = { 0.0f, 0.0f, 0.0f };
float VertexC[3] = { 0.0f, 0.0f, 0.0f };

float VertexA_roof_slide[3] = { -5.0f, 0.0f, 0.0f };
float VertexB_roof_slide[3] = { 0.0f, 10.0f, 0.0f };
float VertexC_roof_slide[3] = { 5.0f, 0.0f, 0.0f };

float N[3] = { 0.0f, 0.0f, 0.0f };
float N_backup[3] = { 0.0f, 0.0f, 0.0f };

int front = 1;

typedef struct point3 {
    float x, y, z;
} point3;

point3 vertices[] = {
    {0.0, 0.0, 1.0},
    {0.0, 0.942809, -0.33333},
    {-0.816497, -0.471405, -0.333333},
    {0.816497, -0.471405, -0.333333}
};

void normalize(point3* p) {
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

void crossProduct(float A[3], float B[3], float result[3]) {
    result[0] = A[1] * B[2] - A[2] * B[1];
    result[1] = A[2] * B[0] - A[0] * B[2];
    result[2] = A[0] * B[1] - A[1] * B[0];
}

void calculateVector(float A[3], float B[3], float result[3]) {
    result[0] = B[0] - A[0];
    result[1] = B[1] - A[1];
    result[2] = B[2] - A[2];
}

void normalize_Vector(float vec[3]) {
    float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    vec[0] /= length;
    vec[1] /= length;
    vec[2] /= length;
}

void myInit() {
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-60.0, 60.0, -60.0, 60.0, -200.0, 200.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glNewList(plain, GL_COMPILE);
    glBegin(GL_QUADS);
    VertexA[0] = -50.0;  VertexA[1] = 0.0;  VertexA[2] = -50.0;
    VertexB[0] = 50.0; VertexB[1] = 0.0; VertexB[2] = -50.0;
    VertexC[0] = 50.0; VertexC[1] = 0.0; VertexC[2] = 50.0;
    calculateVector(VertexA, VertexB, A);
    calculateVector(VertexA, VertexC, B);
    crossProduct(B, A, N);
    normalize_Vector(N); // Add this line
    N_backup[0] = N[0];
    N_backup[1] = N[1];
    N_backup[2] = N[2];
    glNormal3f(N[0], N[1], N[2]);
    glVertex3f(-50.0, 0.0, -50.0);
    glVertex3f(50.0, 0.0, -50.0);
    glVertex3f(50.0, 0.0, 50.0);
    glVertex3f(-50.0, 0.0, 50.0);
    glEnd();
    glEndList();

    glNewList(roof_sides, GL_COMPILE);
    glBegin(GL_TRIANGLES);

    // Calculate vectors from A to B and from A to C
    float A[3], B[3];

    // Calculate the normal
    float N[3];

    calculateVector(VertexA_roof_slide, VertexB_roof_slide, A);
    calculateVector(VertexA_roof_slide, VertexC_roof_slide, B);
    
    // Set the normal
    crossProduct(B, A, N);

    // Normalize the normal
    normalize(N);
    
    glNormal3f(N[0], N[1], N[2]);

    // Define the vertices of the triangle
    glVertex3fv(VertexA_roof_slide);
    glVertex3fv(VertexB_roof_slide);
    glVertex3fv(VertexC_roof_slide);

    glEnd();
    glEndList();


    glNewList(roof_sides_2, GL_COMPILE);
    glBegin(GL_TRIANGLES);

    // Calculate vectors from A to B and from A to C

    calculateVector(VertexA_roof_slide, VertexB_roof_slide, A);
    calculateVector(VertexA_roof_slide, VertexC_roof_slide, B);

    // Calculate the normal
    crossProduct(A, B, N);

    // Normalize the normal
    normalize(N);

    glNormal3f(N[0], N[1], N[2]);

    // Define the vertices of the triangle
    glVertex3fv(VertexA_roof_slide);
    glVertex3fv(VertexB_roof_slide);
    glVertex3fv(VertexC_roof_slide);

    glEnd();
    glEndList();


    glNewList(roof, GL_COMPILE);
    glBegin(GL_QUADS);

    // Define the vertices
    VertexA[0] = -5.5;  VertexA[1] = 0.0;  VertexA[2] = 10.0;
    VertexB[0] = 5.5; VertexB[1] = 0.0; VertexB[2] = 10.0;
    VertexC[0] = 5.5; VertexC[1] = 0.0; VertexC[2] = -10.0;

    // Calculate vectors from A to B and from A to C
    calculateVector(VertexA, VertexB, A);
    calculateVector(VertexA, VertexC, B);

    // Calculate the normal
    crossProduct(B, A, N);

    // Normalize the normal
    normalize(N);

    // Set the normal
    glNormal3f(N[0], N[1], N[2]);

    // Define the vertices of the quad
    glVertex3f(-5.5f, 0.0f, 10.0f);   // Top-left vertex-from 5 to 5.5 to make the foor a bit biger to match the triangles height
    glVertex3f(5.5f, 0.0f, 10.0f);    // Top-right vertex
    glVertex3f(5.5f, 0.0f, -10.0f);   // Bottom-right vertex
    glVertex3f(-5.5f, 0.0f, -10.0f);  // Bottom-left vertex

    glEnd();
    glEndList();

    glNewList(main_build, GL_COMPILE);
    // Define the cube vertices
    GLfloat vertices[8][3] = {
        {-5.0f, -5.0f, 10.0f}, // 0
        {5.0f, -5.0f, 10.0f},  // 1
        {5.0f, 5.0f, 10.0f},   // 2
        {-5.0f, 5.0f, 10.0f},  // 3
        {-5.0f, -5.0f, -10.0f}, // 4
        {5.0f, -5.0f, -10.0f},  // 5
        {5.0f, 5.0f, -10.0f},   // 6
        {-5.0f, 5.0f, -10.0f}  // 7
    };

    // Define the cube faces
    int faces[6][4] = {
        {0, 3, 2, 1}, // Front
        {1, 5, 6, 2}, // Right
        {5, 4, 7, 6}, // Back
        {4, 0, 3, 7}, // Left
        {3, 2, 6, 7}, // Top
        {4, 5, 1, 0}  // Bottom
    };


    glPushMatrix();

    for (int i = 0; i < 6; i++) {
        // Calculate vectors
        float A[3], B[3];
        calculateVector(vertices[faces[i][0]], vertices[faces[i][1]], A);
        calculateVector(vertices[faces[i][0]], vertices[faces[i][3]], B);

        // Calculate the normal
        float N[3];
        if (i != 2) {
            crossProduct(A, B, N);
        }
        else {
            crossProduct(B, A, N);
        }

        // Normalize the normal
        normalize_Vector(N);

        glBegin(GL_QUADS);
        glNormal3f(N[0], N[1], N[2]);
        for (int j = 0; j < 4; j++) {
            glVertex3fv(vertices[faces[i][j]]);
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();

}

void drawHouse() {
    // Set material properties for the main building (matte surface)
    GLfloat main_building_diffuse[] = { 0.941f, 0.0f, 0.0f, 1.0f }; // Arctic white color
    GLfloat main_building_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat main_building_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat main_building_shininess = 0.0f;

    // Set material properties for the roof (metallic surface)
    GLfloat roof_diffuse[] = { 0.329412f, 0.329412f, 0.329412f, 1.0f }; // Grey color
    GLfloat roof_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat roof_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular highlights
    GLfloat roof_shininess = 100.0f;

    // Set material properties for the main building
    glMaterialfv(GL_FRONT, GL_DIFFUSE, main_building_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, main_building_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, main_building_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, main_building_shininess);

    glPushMatrix();
    glTranslatef(0.0f, -5.0f, 0.0f);
    glCallList(main_build);
    glPopMatrix();

    // Set material properties for the roof
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roof_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roof_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roof_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, roof_shininess);

    // Draw the triangular "roof"
    glPushMatrix();
    glTranslatef(-2.3, 5.0, 0.0); // Position the roof above the main building
    glRotatef(-180.0, 80.0, 50.0, 0.0); // Rotate the roof to make it triangular --changed angle a bit
    glCallList(roof);
    glPopMatrix();

    // Draw the triangular "roof"
    glPushMatrix();
    glTranslatef(2.3, 5.0, 0.0); // Position the roof above the main building
    glRotatef(-180.0, -80.0, 50.0, 0.0); // Rotate the roof to make it triangular  --changed angle a bit
    glCallList(roof);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, -10.0); // Position the roof above the main building
    glCallList(roof_sides);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 10.0); // Position the roof above the main building
    glCallList(roof_sides_2);
    glPopMatrix();

    glutPostRedisplay();
}

void drawGrass() {
    GLfloat mat_ambient[] = { 0.0, 1.0, 0.0, 1.0 }; // Green
    GLfloat mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 }; // Green
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 }; // No specular reflection
    GLfloat mat_shininess[] = { 0.0 }; // No shininess

    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    if (polygon == 0) {
        glPushMatrix();
        glTranslatef(0.0, -10.0, 0.0);
        glCallList(plain);
        glPopMatrix();
    }
    else {
        glNormal3f(N[0], N[1], N[2]); // Set the normal for lighting calculations
        // Draw 100 smaller polygons on the y = -10 plane
        for (int i = -5; i < 5; i++) {
            for (int j = -5; j < 5; j++) {
                float x = i * 10.0f;
                float z = j * 10.0f;

                glBegin(GL_QUADS);

                glVertex3f(x, -10.0f, z);
                glVertex3f(x + 10.0f, -10.0f, z);
                glVertex3f(x + 10.0f, -10.0f, z + 10.0f);
                glVertex3f(x, -10.0f, z + 10.0f);
                glEnd();
            }
        }
    }
    glutPostRedisplay();
}

void drawSun() {

    glPushMatrix();

    glTranslatef(sphere_x, sphere_y, sphere_z);

    // Set material properties for the sphere (emissive yellow surface)
    GLfloat sphere_emission[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow color
    GLfloat sphere_zero[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Zero reflection

    // Set material properties for the sphere
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sphere_zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sphere_zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sphere_emission);

    // draw tetrahedron
    divide_triangle(vertices[0], vertices[1], vertices[2], 4);
    divide_triangle(vertices[3], vertices[2], vertices[1], 4);
    divide_triangle(vertices[0], vertices[3], vertices[1], 4);
    divide_triangle(vertices[0], vertices[2], vertices[3], 4);

    // Reset emission color after drawing the sun
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission);

    glPopMatrix();

    glutPostRedisplay();
}


void moveCamera(int key, int x, int y) {
    const float rotationSpeed = 2.0f;  // Speed of rotation
    float radius = 70.0;//find radius for rotation around center

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
    gluLookAt(cameraX, 40.0, cameraZ,  // camera position
        0.0f, 0.0f, 0.0f, // point to look at
        0.0f, 1.0f, 0.0f); // up

    // Draw the grass
    drawGrass();

    // Draw the house
    drawHouse();

    // Draw the sun
    drawSun();

    if (smooth == 1)
        glShadeModel(GL_SMOOTH);  // Enable smooth shading
    else
        glShadeModel(GL_FLAT);  // Enable smooth shading

    if (spotlight_trigger == 1) {
        // Setup light
        GLfloat spotlightPosition[] = { 0.0, 15.0, 20.0, 1.0 }; // Position at the corner of the house
        GLfloat spotlightDirection[] = { 0.0, -1.0, 0.3 }; // Pointing down
        GLfloat yellowLight[] = { 1.0, 1.0, 0.0, 1.0 }; // yellow light

        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_POSITION, spotlightPosition);
        glLightfv(GL_LIGHT1, GL_AMBIENT, yellowLight);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);
        glLightfv(GL_LIGHT1, GL_SPECULAR, yellowLight);

        // Setting spotlight parameters
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f); // Cutoff angle
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightDirection); // Direction of the light
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0); // Sharpness of the spotlight edge
    }
    else {
        glDisable(GL_LIGHT1);
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback function for menu
void processMenu(int option) {
    if (option == 7) {
        exit(0);
    }
}

// Callback function for submenu
void processSubMenu(int option) {
    // switch case for different options
    switch (option) {
    case 1:
        polygon = 0;
        break;
    case 2:
        polygon = 1;
        break;
    case 3:
        smooth = 0;
        break;
    case 4:
        smooth = 1;
        break;
    case 5:
        spotlight_trigger = 1;
        break;
    case 6:
        spotlight_trigger = 0;
        break;
    }
    glutPostRedisplay();
}

void createGLUTMenus() {
    // Create submenu
    int PolygonsubMenu = glutCreateMenu(processSubMenu);
    glutAddMenuEntry("Low", 1);
    glutAddMenuEntry("High", 2);

    // Create submenu
    int shadesubMenu = glutCreateMenu(processSubMenu);
    glutAddMenuEntry("Flat", 3);
    glutAddMenuEntry("Smooth", 4);

    // Create submenu
    int spotlightsubMenu = glutCreateMenu(processSubMenu);
    glutAddMenuEntry("On", 5);
    glutAddMenuEntry("Off", 6);

    // Create main menu
    int mainMenu = glutCreateMenu(processMenu);
    glutAddSubMenu("Polygon", PolygonsubMenu);
    glutAddSubMenu("Shade", shadesubMenu);
    glutAddSubMenu("Spotlight", spotlightsubMenu);
    glutAddMenuEntry("Quit", 7);

    // Attach the menu to the right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void idle() {
    // Calculate the new position of the sphere
    shpere_angle += 0.0005f;

    sphere_x = 50.0f * cos(shpere_angle);
    sphere_y = 50.0f * sin(shpere_angle);

    light_position[0] = sphere_x;
    light_position[1] = sphere_y;

    float light_direction[3] = { -sphere_x, -sphere_y, -0.0f };

    // When the sphere reaches the other end, reset angle
    if (shpere_angle > M_PI) {
        shpere_angle = 0.0f;
    }


    // Update the diffuse light intensity based on the time
    float intensity = 0.3f + 0.7f * sin(shpere_angle);
    light_diffuse[0] = intensity;
    light_diffuse[1] = intensity;
    light_diffuse[2] = intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);


    // Redisplay
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Project-3");
    glEnable(GL_DEPTH_TEST);

    myInit();
    createGLUTMenus(); // create menu and options

    glutSpecialFunc(moveCamera);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);


    glutMainLoop();

    return 0;
}
