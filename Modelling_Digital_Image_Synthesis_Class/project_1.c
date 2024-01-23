#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>


// GLOBAL VARIABLES

int question = 1;

// Variables for control points
float controlPoints[7][3];
int pointCount = 0;
bool drawEnabled = false;
int selectedPoint = -1;

// Function to interpolate between two points
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
    for (int i = 0; i < degree; ++i) {
        interpolate(newpoints[i], points[i], points[i + 1], t);
    }

    // Recursively call deCasteljau on the new set of interpolated points
    deCasteljau(dest, t, newpoints, degree - 1);
}

// Function to check distance for selection of a point
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void drawCurveQuestion1() {
    if (drawEnabled) {
        // Draw the first curve
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);
        glEnable(GL_MAP1_VERTEX_3);

        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++) {
            glEvalCoord1f((GLfloat)i / 30.0);
        }
        glEnd();

        glColor3f(0.0, 1.0, 0.0);
        // Draw the second curve
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[3][0]);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++) {
            glEvalCoord1f((GLfloat)i / 30.0);
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
void drawCurveQuestion2() {
    if (drawEnabled) {
        // Κλειστή καμπύλη Bezier - εξασφαλίζουμε ότι το πρώτο και τελευταίο σημείο ελέγχου συμπίπτουν
        controlPoints[6][0] = controlPoints[0][0];
        controlPoints[6][1] = controlPoints[0][1];
        controlPoints[6][2] = controlPoints[0][2];

        /*
        // Σχεδίαση της καμπύλης Bezier
        glEnable(GL_MAP1_VERTEX_3);
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 7, &controlPoints[0][0]);

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++) {
            glEvalCoord1f((GLfloat)i / 30.0);
        }
        glEnd();
        */

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

void drawBezierCurveSegment(float controlPoints[4][3]) {
    // Assuming the control points are correctly set for C0 and C1 continuity
    glEnable(GL_MAP1_VERTEX_3);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 30; i++) {
        glEvalCoord1f((GLfloat)i / 30.0);
    }
    glEnd();
}

void drawCurveQuestion3() {
    
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    if (question == 1) {
        drawCurveQuestion1();
    }

    if (question == 2) {
        drawCurveQuestion2();
    }

    if (question == 3) {
        drawCurveQuestion3();
    }

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

// Ενημερώνει το σημείο που έχει επιλεγεί με βάση τη θέση του ποντικιού
void updatePointPosition(int x, int y) {
    if (selectedPoint != -1) {
        double w = glutGet(GLUT_WINDOW_WIDTH);
        double h = glutGet(GLUT_WINDOW_HEIGHT);
        controlPoints[selectedPoint][0] = (x - w / 2) * 20.0 / w;
        controlPoints[selectedPoint][1] = (h / 2 - y) * 20.0 / h;
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    // Ορίζουμε την απόσταση ανίχνευσης
    const float d = 10;
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    float worldX = (x - w / 2) * 20.0 / w;
    float worldY = (h / 2 - y) * 20.0 / h;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (!drawEnabled) {
                // Προσθήκη σημείων ελέγχου αν δεν έχουμε ακόμα φτάσει τα 7
                if (pointCount < 7) {
                    controlPoints[pointCount][0] = worldX;
                    controlPoints[pointCount][1] = worldY;
                    controlPoints[pointCount][2] = 0.0; // Z coordinate is zero for 2D
                    pointCount++;
                    if (pointCount == 7) {
                        drawEnabled = true;
                    }
                }
            }
            else {
                // Εντοπισμός και επιλογή του πλησιέστερου σημείου ελέγχου
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
        else if (state == GLUT_UP) {
            // Απελευθέρωση του επιλεγμένου σημείου
            selectedPoint = -1;
        }
        glutPostRedisplay();
    }
}


void motion(int x, int y) {
    if (selectedPoint != -1) {
        // Convert window coordinates to world coordinates for motion
        double w = glutGet(GLUT_WINDOW_WIDTH);
        double h = glutGet(GLUT_WINDOW_HEIGHT);
        controlPoints[selectedPoint][0] = (x - w / 2) * 20.0 / w;
        controlPoints[selectedPoint][1] = (h / 2 - y) * 20.0 / h;

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

    if (id == 4) {
        question = 4;
        glutPostRedisplay();
    }

    if (id == 5) {
        exit(0);
    }
}

// create menu and options
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("2", 2);
    glutAddMenuEntry("3", 3);
    //glutAddMenuEntry("4", 4);
    glutAddMenuEntry("Quit", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Κυβικές Καμπύλες Παρεμβολής με Έλεγχο Ποντικιού");
    createMenu(); // create menu and options
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion); // Register motion callback
    glutMainLoop();
    return 0;
}
