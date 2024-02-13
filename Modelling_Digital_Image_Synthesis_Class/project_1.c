#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

// Global variable to check which question to implement
int question = 1;

// Variables for control points
float controlPoints[7][3];
int pointCount = 0;
bool drawEnabled = false;
int selectedPoint = -1;

// basis matrix for cubic interpolation (for question 1)
float M[4][4] = {
    {1, 0, 0, 0},
    {-5.5, 9, -4.5, 1},
    {9, -22.5, 18, -4.5},
    {-4.5, 13.5, -13.5, 4.5}
};


// Support function to draw control points
void drawControlPoints() {
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; i++)
        glVertex3fv(&controlPoints[i][0]);
    glEnd();
}

// Support function to get the result a point of the curve (for question 1)
void multiplyMatrixAndVector(float u, float points[4][3], float result[3]) {
    float U[4] = { 1, u, u * u, u * u * u };

    // Initialize the result array to 0
    for (int i = 0; i < 3; i++)
        result[i] = 0;

    // multiplying
    for (int i = 0; i < 4; i++) {
        result[0] += U[i] * (M[i][0] * points[0][0] + M[i][1] * points[1][0] + M[i][2] * points[2][0] + M[i][3] * points[3][0]);
        result[1] += U[i] * (M[i][0] * points[0][1] + M[i][1] * points[1][1] + M[i][2] * points[2][1] + M[i][3] * points[3][1]);
        result[2] += U[i] * (M[i][0] * points[0][2] + M[i][1] * points[1][2] + M[i][2] * points[2][2] + M[i][3] * points[3][2]);
    }
}

// Support function to draw a curve (for question 1)
void drawCubicCurve(float points[4][3]) {
    glBegin(GL_LINE_STRIP); // Start drawing a line strip

    float curvePoint[3]; // Temporary storage for the curve point
    // Iterate over u from 0 to 1 to generate the curve
    for (float u = 0; u <= 1; u += 0.01) {
        multiplyMatrixAndVector(u, points, curvePoint); // Fill curvePoint with the computed coordinates
        glVertex3f(curvePoint[0], curvePoint[1], curvePoint[2]); // Create a vertex at the calculated point
    }

    glEnd(); // End drawing the line strip
}

void drawQuestion1() {
    
    if (drawEnabled) {
        float segment[4][3]; // Temporary storage for a segment of four control points

        // Prepare and draw the first cubic curve segment
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
                segment[i][j] = controlPoints[i][j];

        glColor3f(0.0, 1.0, 0.0);
        drawCubicCurve(segment);

        // Prepare and draw the second cubic curve segment
        // Note: The second segment starts at controlPoints[3], so it includes controlPoints[3] to controlPoints[6]
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
                segment[i][j] = controlPoints[i + 3][j]; // Adjust index to get the second segment

        glColor3f(0.0, 0.0, 1.0);
        drawCubicCurve(segment);
    }

    drawControlPoints();
}

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

void drawCurveQuestion2() {
    if (drawEnabled) {
        // First and last point are the same
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

    drawControlPoints();
}

void drawBezierCurve(float points[4][3]) {
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, points);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 30; i++)
        glEvalCoord1f((GLfloat)i / 30.0);
    glEnd();
}

void drawCurveQuestion3() {

    if (drawEnabled) {

        glEnable(GL_MAP1_VERTEX_3);

        // Draw the first curve
        float segment[4][3]; // Temporary storage for a segment of four control points

        // Prepare and draw the first cubic curve segment
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
                segment[i][j] = controlPoints[i][j];

        glColor3f(0.0, 1.0, 0.0);
        drawBezierCurve(segment);

        // Draw the second curve

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
                segment[i][j] = controlPoints[i + 3][j]; // Adjust index to get the second segment

        glColor3f(0.0, 0.0, 1.0);
        drawBezierCurve(segment);
    }

    drawControlPoints();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    if (question == 1)
        drawQuestion1();

    if (question == 2)
        drawCurveQuestion2();

    if (question == 3)
        drawCurveQuestion3();

    glFlush();
}

// Function to check distance for selection of a point
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
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

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    glutCreateWindow("Curves");
    createMenu(); // create menu and options
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion); // Register motion callback
    glutMainLoop();
    return 0;
}
