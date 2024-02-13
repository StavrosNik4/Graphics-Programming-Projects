#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>


#define M_PI  3.14159265358979323846

// Camera global variables
float angle = 0.0f;
float cameraX = 0.0;
float cameraZ = 20.0;

int surfaceType = 1;

// basis matrix for cubic interpolation (for question 1)
float M[4][4] = {
    {1, 0, 0, 0},
    {-5.5, 9, -4.5, 1},
    {9, -22.5, 18, -4.5},
    {-4.5, 13.5, -13.5, 4.5}
};

float MT[4][4] = {
    {1, -5.5, 9, -4.5},
    {0, 9, -22.5, 13.5},
    {0, -4.5, 18, -13.5},
    {0, 1, -4.5, 4.5}
};

// Assume controlPoints is a 4x4x3 array of floats now, representing a 4x4 grid of 3D control points.
float surfaceControlPoints[4][4][3] = {
    {
        {0.0, 0.0, 0.0}, // P00
        {1.0, 0.0, 0.0}, // P01
        {2.0, 0.0, 0.0}, // P02
        {3.0, 0.0, 0.0}  // P03
    },
    {
        {0.0, 1.0, 0.0}, // P10
        {1.0, 1.0, 0.0}, // P11
        {2.0, 1.0, 0.0}, // P12
        {3.0, 1.0, 0.0}  // P13
    },
    {
        {0.0, 2.0, 0.0}, // P20
        {1.0, 2.0, 0.0}, // P21
        {2.0, 2.0, 0.0}, // P22
        {3.0, 2.0, 0.0}  // P23
    },
    {
        {0.0, 3.0, 0.0}, // P30
        {1.0, 3.0, 0.0}, // P31
        {2.0, 3.0, 0.0}, // P32
        {3.0, 3.0, 0.0}  // P33
    }
};

float wavySurfaceControlPoints[4][4][3] = {
                {
                    {0.0, 0.0, 0.0}, // P00
                    {1.0, 0.0, 1.0}, // P01
                    {2.0, 0.0, 0.0}, // P02
                    {3.0, 0.0, -1.0} // P03
                },
                {
                    {0.0, 1.0, -1.0}, // P10
                    {1.0, 1.0, 0.0}, // P11
                    {2.0, 1.0, 1.0}, // P12
                    {3.0, 1.0, 0.0}  // P13
                },
                {
                    {0.0, 2.0, 1.0}, // P20
                    {1.0, 2.0, -1.0}, // P21
                    {2.0, 2.0, 0.0}, // P22
                    {3.0, 2.0, 1.0}  // P23
                },
                {
                    {0.0, 3.0, 0.0}, // P30
                    {1.0, 3.0, 1.0}, // P31
                    {2.0, 3.0, -1.0}, // P32
                    {3.0, 3.0, 0.0}  // P33
                }
};

void updateSurfaceControlPoints() {
    switch (surfaceType) {
    case 1:
        // Flat surface initialization
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                surfaceControlPoints[i][j][0] = (float)j; // x
                surfaceControlPoints[i][j][1] = (float)i; // y
                surfaceControlPoints[i][j][2] = 0.0f;     // z
            }
        }
        break;

    case 2:
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 3; k++) {
                    surfaceControlPoints[i][j][k] = wavySurfaceControlPoints[i][j][k];
                }
            }
        }

        break;

        /*
        // Wavy surface initialization
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                surfaceControlPoints[i][j][0] = (float)j; // x
                surfaceControlPoints[i][j][1] = (float)i; // y
                // Alternate the z-coordinate to create a wave pattern
                surfaceControlPoints[i][j][2] = ((i + j) % 2 == 0) ? 1.0f : -1.0f;
            }
        }
        break;*/

    default:
        // Default or no pattern specified
        printf("No such pattern exists. Initializing with pattern 1 as default.\n");
        updateSurfaceControlPoints(1);
        break;
    }
}

// Support function to draw control points
void drawControlPoints() {
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            glVertex3fv(&surfaceControlPoints[i][j]);
    glEnd();
}



void multiplyMatrixAndVectorSurface(float u, float v, float controlPoints[4][4][3], float result[3]) {
    float U[4] = { 1.0, u, u * u, u * u * u };
    float V[4] = { 1.0, v, v * v, v * v * v };
    float UM[4] = { 0.0 };
    float VM[4] = { 0.0 };
    float controlPointsU[4][3] = { 0.0 };
    float controlPointsV[4][3] = { 0.0 };

    for (int i = 0; i < 3; i++)
        result[i] = 0.0f;

    // Apply the basis matrix in the u and v directions
    for (int i = 0; i < 4; i++) {
        UM[i] = U[3] * M[3][i] + U[2] * M[2][i] + U[1] * M[1][i] + U[0] * M[0][i];
        VM[i] = V[3] * M[3][i] + V[2] * M[2][i] + V[1] * M[1][i] + V[0] * M[0][i];
    }

    // Compute the interpolated control points in the u direction
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                controlPointsU[i][j] += UM[k] * controlPoints[k][i][j];
            }
        }
    }

    // Compute the final interpolated point in the v direction
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            result[i] += controlPointsU[j][i] * VM[j];
        }
    }
}

void drawBicubicSurface() {

    glPointSize(1.0);

    glColor3f(0.0, 1.0, 0.0);

    for (float u = 0; u <= 1; u += 0.02) {
        for (float v = 0; v <= 1; v += 0.02) {
            float surfacePoint[3];
            multiplyMatrixAndVectorSurface(u, v, surfaceControlPoints, surfacePoint);
            // Now, you would draw this point on the surface
            // For example, you might use glVertex3fv(surfacePoint) inside a glBegin(GL_POINTS) block
            glBegin(GL_POINTS);
                glVertex3fv(surfacePoint);
            glEnd();
        }
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW); // Switch to the drawing perspective
    glLoadIdentity(); // Reset the drawing perspective

    // Position the camera
    gluLookAt(cameraX, 10.0, cameraZ,  // Camera position in World Space
        0.0, 0.0, 0.0,           // Look at in the center of the scene
        0.0, 1.0, 0.0);          // Up vector is (0,1,0) (positive Y-axis)

    glColor3f(0.0, 1.0, 0.0); // Set the color of the surface to green

    drawBicubicSurface(); // Draw the surface

    drawControlPoints(); // Draw the control points

    glFlush(); // Flush the rendered output to the screen
}


void reshape(int w, int h) {
    if (h == 0) h = 1; // Prevent division by zero
    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}


// Function for the movement of the camera
void moveCamera(int key, int x, int y) {
    const float rotationSpeed = 2.0f;  // Speed of rotation
    float radius = 20; //find radius for rotation around center

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

// menu callback function
void menu(int id)
{

    if (id == 1) {
        surfaceType = 1;
        updateSurfaceControlPoints();
        glutPostRedisplay();
    }

    if (id == 2) {
        surfaceType = 2;
        updateSurfaceControlPoints();
        glutPostRedisplay();
    }

    if (id == 5) exit(0);

}

// create menu and options
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Plain", 1);
    glutAddMenuEntry("Wavy", 2);
    glutAddMenuEntry("Quit", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}


void init() {
    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Set the background color to black (or any other color)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Curves");

    init();

    createMenu(); // create menu and options

    glutSpecialFunc(moveCamera);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
