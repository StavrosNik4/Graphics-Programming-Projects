#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define M_PI  3.14159265358979323846

// Structure to represent each dog part
typedef struct DogPart {
    GLfloat m[16];
    void (*f)();
    struct DogPart* sibling;
    struct DogPart* child;
} DogPart;

// Initialize the dog parts
DogPart body, neck, head, upper_limb_A, lower_limb_A, paw_A, upper_limb_B, lower_limb_B, paw_B, upper_limb_C, lower_limb_C, paw_C, upper_limb_D, lower_limb_D, paw_D;

// Function to set an identity matrix
void setIdentityMatrix(GLfloat* matrix) {
    for (int i = 0; i < 16; i++) {
        matrix[i] = 0.0f;
    }
    // Set diagonal to 1
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

void initializeMatrices() {
    setIdentityMatrix(body.m);
    setIdentityMatrix(neck.m);
    setIdentityMatrix(head.m);
    setIdentityMatrix(upper_limb_A.m);
    setIdentityMatrix(lower_limb_A.m);
    setIdentityMatrix(paw_A.m);
    setIdentityMatrix(upper_limb_B.m);
    setIdentityMatrix(lower_limb_B.m);
    setIdentityMatrix(paw_B.m);
    setIdentityMatrix(upper_limb_C.m);
    setIdentityMatrix(lower_limb_C.m);
    setIdentityMatrix(paw_C.m);
    setIdentityMatrix(upper_limb_D.m);
    setIdentityMatrix(lower_limb_D.m);
    setIdentityMatrix(paw_D.m);
}

void createTree() {
    // Body has no siblings and neck as a child
    body.sibling = NULL;
    body.child = &neck;

    // Neck has upper_limb_A as a sibling and head as a child
    neck.sibling = &upper_limb_A;
    neck.child = &head;

    // Head has no siblings and no children
    head.sibling = NULL;
    head.child = NULL;

    // Upper limb A has upper_limb_B as a sibling and lower_limb_A as a child
    upper_limb_A.sibling = &upper_limb_B;
    upper_limb_A.child = &lower_limb_A;

    // Lower limb A has no siblings and paw_A as a child
    lower_limb_A.sibling = NULL;
    lower_limb_A.child = &paw_A;

    // Paw A has no siblings and no children
    paw_A.sibling = NULL;
    paw_A.child = NULL;

    // Repeat the same pattern for the other limbs
    upper_limb_B.sibling = &upper_limb_C;
    upper_limb_B.child = &lower_limb_B;

    lower_limb_B.sibling = NULL;
    lower_limb_B.child = &paw_B;

    paw_B.sibling = NULL;
    paw_B.child = NULL;

    upper_limb_C.sibling = &upper_limb_D;
    upper_limb_C.child = &lower_limb_C;

    lower_limb_C.sibling = NULL;
    lower_limb_C.child = &paw_C;

    paw_C.sibling = NULL;
    paw_C.child = NULL;

    upper_limb_D.sibling = NULL; // Last limb, no sibling
    upper_limb_D.child = &lower_limb_D;

    lower_limb_D.sibling = NULL;
    lower_limb_D.child = &paw_D;

    paw_D.sibling = NULL;
    paw_D.child = NULL;

}

GLUquadric* quadric;

void initQuadric() {
    quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

void drawLimbNeck() {
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(quadric, 0.1, 0.1, 0.2, 32, 32); // Example dimensions
}

void drawUpperLimb() {
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(quadric, 0.1, 0.1, 0.4, 32, 32); // Example dimensions
}

void drawLowerLimb() {
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(quadric, 0.08, 0.08, 0.3, 32, 32); // Example dimensions
}

void drawPaw() {
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(quadric, 0.08, 0.08, 0.3, 32, 32); // Example dimensions
}

// Function to draw the body
void drawBody() {
    glColor3f(1.0, 0.0, 0.0);
    gluCylinder(quadric, 0.3, 0.3, 1.0, 32, 32); // Example dimensions
}

// Function to draw the head as a sphere
void drawHead() {
    glColor3f(0.0, 0.0, 1.0);
    gluSphere(quadric, 0.25, 32, 32); // Example dimensions
}



void assignDrawingFunctions() {
    body.f = drawBody;
    neck.f = drawLimbNeck;
    head.f = drawHead;
    upper_limb_A.f = drawUpperLimb;
    lower_limb_A.f = drawLowerLimb;
    paw_A.f = drawPaw;
    upper_limb_B.f = drawUpperLimb;
    lower_limb_B.f = drawLowerLimb;
    paw_B.f = drawPaw;
    upper_limb_C.f = drawUpperLimb;
    lower_limb_C.f = drawLowerLimb;
    paw_C.f = drawPaw;
    upper_limb_D.f = drawUpperLimb;
    lower_limb_D.f = drawLowerLimb;
    paw_D.f = drawPaw;
}

// Recursive function to draw each part
void drawDogPart(DogPart* part) {
    if (part == NULL) return;

    glPushMatrix(); // Save the current transformation

    // Apply this part's transformation
    glMultMatrixf(part->m);

    // Draw this part
    part->f();

    // Recursively draw the child parts
    if (part->child != NULL) {
        drawDogPart(part->child);
    }

    glPopMatrix(); // Restore the transformation

    // Draw the sibling parts
    if (part->sibling != NULL) {
        drawDogPart(part->sibling);
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update gluLookAt with dynamic cameraX and cameraZ values
    gluLookAt(cameraX, 0.0, cameraZ,  // eye position
        0.0, 0.0, 0.0,           // center position (looking at)
        0.0, 1.0, 0.0);          // up vector

    glColor3f(1.0, 1.0, 1.0);

    drawDogPart(&body);


    glutSwapBuffers();
}

void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0); // Adjust field of view, aspect ratio, near and far clipping planes
    glMatrixMode(GL_MODELVIEW);
}



// Function to handle the window reshape properly
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void applyTransformations() {
    // Body is at the origin, so no need to move it
    // Apply rotation to the body if needed, for example:
    //rotateMatrix(body.m, 90.0f, 0.0f, 1.0f, 0.0f); // If you want to rotate the body

    // Neck
    // Translate the neck to be at the top of the body
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.25f, 1.0f); // Adjust these values as needed
    glRotatef(-25.0f, 1.0f, 0.0f, 0.0f); // Slight forward tilt
    glGetFloatv(GL_MODELVIEW_MATRIX, neck.m);
    glPopMatrix();

    // Head
    // Translate the head to be at the top of the neck
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.1f, 0.4f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, head.m);
    glPopMatrix();

    // Upper limbs
    // Translate and rotate each upper limb to the correct position relative to the body
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.3f, -0.3f, 0.3f); // Left front limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.3f, -0.3f, 0.3f); // Right front limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.3f, -0.3f, -0.3f); // Left front limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.3f, -0.3f, -0.3f); // Right front limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_D.m);
    glPopMatrix();



    /*
    // Lower limbs
    // Translate each lower limb to be at the bottom of the upper limb
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, -0.4f, 0.0f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_A.m);
    glPopMatrix();

    // Repeat the process for lower limbs B, C, and D

    // Paws
    // Translate each paw to be at the bottom of the lower limb
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, -0.3f, 0.0f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_A.m);
    glPopMatrix();

    // Repeat the process for paws B, C, and D
    */
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("What the dog doin?");

    setupProjection();

    // Set the display function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    

    initializeMatrices();
    createTree();
    initQuadric();
    assignDrawingFunctions();

    // Call this function after initializing matrices and before entering the GLUT main loop
    applyTransformations();


    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}
