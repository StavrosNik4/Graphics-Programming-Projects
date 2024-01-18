#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

// Variables for control points
float controlPoints[7][3];
int pointCount = 0;
bool drawEnabled = false;
int selectedPoint = -1;

// Function to check distance for selection of a point
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void drawCurve() {
    if (drawEnabled) {
        // Draw the first curve
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);
        glEnable(GL_MAP1_VERTEX_3);

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++) {
            glEvalCoord1f((GLfloat)i / 30.0);
        }
        glEnd();

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    drawCurve();

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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Κυβικές Καμπύλες Παρεμβολής με Έλεγχο Ποντικιού");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion); // Register motion callback
    glutMainLoop();
    return 0;
}
