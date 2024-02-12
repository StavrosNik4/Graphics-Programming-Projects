#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

// Global variable to check which question to implement
int question = 1;

// Variables for control points
float controlPoints[7][3];
float controlPointsTangents[7][3];
int pointCount = 0;
bool drawEnabled = false;
int selectedPoint = -1;

// Function to interpolate between two points. Used in deCasteljau
void interpolate(float* dest, float* a, float* b, float t) {
    dest[0] = (1 - t) * a[0] + t * b[0];
    dest[1] = (1 - t) * a[1] + t * b[1];
    dest[2] = (1 - t) * a[2] + t * b[2];
}

// de Casteljau's recursive function to compute a point on the Bezier curve
void deCasteljau(float* dest, float t, float points[][3], int degree) {
    // We'll allocate a fixed-size array based on the maximum degree we expect.
    // For a 6th-degree Bezier curve, we need a maximum of 6 interpolations.
    float newpoints[6][3]; // This will work for up to a 6th-degree curve

    // If we only have two points left, interpolate between them and return
    if (degree == 1) {
        interpolate(dest, points[0], points[1], t);
        return;
    }

    // Interpolate between each pair of points
    for (int i = 0; i < degree; ++i)
        interpolate(newpoints[i], points[i], points[i + 1], t);

    // Recursively call deCasteljau on the new set of interpolated points
    deCasteljau(dest, t, newpoints, degree - 1);
}

// Function to check distance for selection of a point
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void calculateTangentsOfControlPoints() {
    int i;
    for (i = 1; i < 7 - 1; i++) {
        controlPointsTangents[i][0] = (controlPoints[i + 1][0] - controlPoints[i - 1][0]) / 2.0;
        controlPointsTangents[i][1] = (controlPoints[i + 1][1] - controlPoints[i - 1][1]) / 2.0;
        controlPointsTangents[i][2] = (controlPoints[i + 1][2] - controlPoints[i - 1][2]) / 2.0;
    }
    // Tangents for the first point
    controlPointsTangents[0][0] = controlPoints[1][0] - controlPoints[0][0];
    controlPointsTangents[0][1] = controlPoints[1][1] - controlPoints[0][1];
    controlPointsTangents[0][2] = controlPoints[1][2] - controlPoints[0][2];

    // Tangents for the last point
    int last = 7 - 1;
    controlPointsTangents[last][0] = controlPoints[last][0] - controlPoints[last - 1][0];
    controlPointsTangents[last][1] = controlPoints[last][1] - controlPoints[last - 1][1];
    controlPointsTangents[last][2] = controlPoints[last][2] - controlPoints[last - 1][2];
}

void drawQuestion1CubicHermiteCurve() {
    int i;
    float t;
    // Hermite basis functions coefficients
    float h00, h10, h01, h11;
    float p[3]; // Point on the curve

    if (drawEnabled) {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i < 7 - 1; i++) {
            
            // Select color
            if (i < 3) 
                glColor3f(0.0, 1.0, 0.0);
            else
                glColor3f(0.0, 0.0, 1.0);

            for (t = 0; t <= 1; t += 0.01) {
                // Compute basis functions
                h00 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
                h10 = pow(t, 3) - 2 * pow(t, 2) + t;
                h01 = -2 * pow(t, 3) + 3 * pow(t, 2);
                h11 = pow(t, 3) - pow(t, 2);

                // Compute the point
                p[0] = h00 * controlPoints[i][0] + h10 * controlPointsTangents[i][0] +
                    h01 * controlPoints[i + 1][0] + h11 * controlPointsTangents[i + 1][0];
                p[1] = h00 * controlPoints[i][1] + h10 * controlPointsTangents[i][1] +
                    h01 * controlPoints[i + 1][1] + h11 * controlPointsTangents[i + 1][1];
                p[2] = h00 * controlPoints[i][2] + h10 * controlPointsTangents[i][2] +
                    h01 * controlPoints[i + 1][2] + h11 * controlPointsTangents[i + 1][2];

                glVertex3fv(p);
            }
        }
        glEnd();
    }

    // Draw control points
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; i++) {
        glVertex3fv(&controlPoints[i][0]);
    }
    glEnd();
}


void drawCurveQuestion3() {

    if (drawEnabled) {

        glEnable(GL_MAP1_VERTEX_3);

        // Draw the first curve
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);

        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++)
            glEvalCoord1f((GLfloat)i / 30.0);
        glEnd();

        // Draw the second curve

        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[3][0]);
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++)
            glEvalCoord1f((GLfloat)i / 30.0);
        glEnd();
    }

    // Draw control points
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; i++)
        glVertex3fv(&controlPoints[i][0]);
    glEnd();
}
void drawCurveQuestion2() {
    if (drawEnabled) {
        // Κλειστή καμπύλη Bezier - εξασφαλίζουμε ότι το πρώτο και τελευταίο σημείο ελέγχου συμπίπτουν
        controlPoints[6][0] = controlPoints[0][0];
        controlPoints[6][1] = controlPoints[0][1];
        controlPoints[6][2] = controlPoints[0][2];

        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINE_STRIP);
        float point[3]; // Point on the curve
        for (int i = 0; i <= 30; i++) {
            float t = (float)i / 30.0f; // Parameter t
            deCasteljau(point, t, controlPoints, 6); // Compute a point on the curve
            glVertex3fv(point); // Draw the point
        }
        glEnd();

    }

    // Σχεδίαση των σημείων ελέγχου
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; i++) {
        glVertex3fv(&controlPoints[i][0]);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    if (question == 1) {
        drawQuestion1CubicHermiteCurve();
        calculateTangentsOfControlPoints();
    }

    if (question == 2)
        drawCurveQuestion2();

    if (question == 3)
        drawCurveQuestion3();

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    const float d = 3.0f;
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    float worldX = (x - w / 2) * 20.0 / w;
    float worldY = (h / 2 - y) * 20.0 / h;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!drawEnabled) {
            if (pointCount < 7) {
                controlPoints[pointCount][0] = worldX;
                controlPoints[pointCount][1] = worldY;
                controlPoints[pointCount][2] = 0; // Z coordinate is zero for 2D
                pointCount++;

                if (pointCount == 7)
                    drawEnabled = true;

                if (pointCount == 7 && question == 3) {
                    // Correctly adjust for p2, p3, and p4 now being at indices 2, 3, and 4
                    float dx = controlPoints[4][0] - controlPoints[3][0]; // x distance from p3 to p4
                    float dy = controlPoints[4][1] - controlPoints[3][1]; // y distance from p3 to p4

                    // Adjust p2 to be mirrored about p3, equidistant from p3 as p4
                    controlPoints[2][0] = controlPoints[3][0] - dx;
                    controlPoints[2][1] = controlPoints[3][1] - dy;
                }
            }
        }
        else {
            // Identifying and selecting the closest control point
            float minDist = FLT_MAX;
            int closestPoint = -1;
            for (int i = 0; i < pointCount; i++) {
                float dist = distance(worldX, worldY, controlPoints[i][0], controlPoints[i][1]);
                if (dist < d && dist < minDist) {
                    minDist = dist;
                    closestPoint = i;
                }
            }
            selectedPoint = closestPoint;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // Release the selected point
        selectedPoint = -1;
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (selectedPoint != -1) {
        double w = glutGet(GLUT_WINDOW_WIDTH);
        double h = glutGet(GLUT_WINDOW_HEIGHT);

        double oldX = controlPoints[selectedPoint][0];
        double oldY = controlPoints[selectedPoint][1];

        // Update the position of the selected point based on mouse coordinates
        controlPoints[selectedPoint][0] = (x - w / 2) * 20.0 / w;
        controlPoints[selectedPoint][1] = (h / 2 - y) * 20.0 / h;

        double dx = oldX - controlPoints[selectedPoint][0];
        double dy = oldY - controlPoints[selectedPoint][1];

        // Check if the question is equal to 3 for special handling
        if (question == 3) {
            // Handling when point 3 is selected
            if (selectedPoint == 3) {

                // Update points 2 and 4 to be mirrored across point 3

                controlPoints[2][0] = controlPoints[2][0] - dx;
                controlPoints[2][1] = controlPoints[2][1] - dy;

                controlPoints[4][0] = controlPoints[4][0] - dx;
                controlPoints[4][1] = controlPoints[4][1] - dy;

            }
            // Handling when point 2 or 4 is selected
            else if (selectedPoint == 2 || selectedPoint == 4) {
                int mirrorPoint = (selectedPoint == 2) ? 4 : 2; // Determine the point to mirror
                int pivotPoint = 3; // Assuming point 3 is the pivot for mirroring

                // Apply the mirroring formula
                controlPoints[mirrorPoint][0] = 2 * controlPoints[pivotPoint][0] - controlPoints[selectedPoint][0];
                controlPoints[mirrorPoint][1] = 2 * controlPoints[pivotPoint][1] - controlPoints[selectedPoint][1];
            }
        }

        // Redraw the scene with the updated points
        glutPostRedisplay();
    }
}

// menu callback function
void menu(int id)
{

    if (id == 1) {
        question = 1;
        pointCount = 0;
        drawEnabled = 0;
        glutPostRedisplay();
    }

    if (id == 2) {
        question = 2;
        pointCount = 0;
        drawEnabled = 0;
        glutPostRedisplay();
    }

    if (id == 3) {
        question = 3;
        pointCount = 0;
        drawEnabled = 0;
        glutPostRedisplay();
    }

    if (id == 5) exit(0);

}

// create menu and options
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("2", 2);
    glutAddMenuEntry("3", 3);
    glutAddMenuEntry("Quit", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Curves and Surface");
    createMenu(); // create menu and options
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion); // Register motion callback
    glutMainLoop();
    return 0;
}
