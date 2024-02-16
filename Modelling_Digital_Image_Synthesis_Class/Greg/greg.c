#include "greg.h"

// Camera Global Variables
float cameraAngle = 0.0f;
float cameraX = 5.0f;
float cameraZ = 0.0f;

// Animation Global Variables
int animation_id = 0;
int isAnimating = 0;

float animationSpeed = 1.0f; // Adjust as needed
float animationTimer = 0.0;
int animationDuration = 100; // Adjust as needed

// neck animation related 
float neckBendAngle = 0.0f;
float originalNeckBendAngle = 0.0f;

// leg animation related
float upperLimbAngle = 0.0f;
float lowerLimbAngle = 0.0f;
float pawAngle = 0.0f;
float originalUpperLimbAngle = 0.0f;
float originalLowerLimbAngle = 0.0f;
float originalPawAngle = 0.0f;

// Animation types
enum AnimationType { START, FINISH };
enum AnimationType animationType = START;

// Define the dog parts
DogPart body, neck, head,
upper_limb_A, lower_limb_A, paw_A,
upper_limb_B, lower_limb_B, paw_B,
upper_limb_C, lower_limb_C, paw_C,
upper_limb_D, lower_limb_D, paw_D;

// function to create the tree of Greg
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

// Function to set an identity matrix to a part's matrix 
void setIdentityMatrix(GLfloat* matrix) {
    for (int i = 0; i < 16; i++) {
        matrix[i] = 0.0f;
    }
    // Set diagonal to 1
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

GLUquadric* quadric;

void initQuadric() {
    quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

// Drawing functions

void drawBody() {
    glColor3f(1.0, 0.0, 0.0); // red
    gluCylinder(quadric, 0.3, 0.3, 1.0, 32, 32);
}

void drawNeck() {
    glColor3f(1.0, 1.0, 1.0); // white
    gluCylinder(quadric, 0.1, 0.1, 0.2, 32, 32);
}

void drawHead() {
    glColor3f(0.0, 0.0, 1.0); // blue
    gluSphere(quadric, 0.25, 32, 32);
}

void drawUpperLimb() {
    glColor3f(0.0, 1.0, 0.0); // green
    gluCylinder(quadric, 0.1, 0.1, 0.4, 32, 32);
}

void drawLowerLimb() {
    glColor3f(0.0, 0.0, 1.0); // blue
    gluCylinder(quadric, 0.08, 0.08, 0.3, 32, 32);
}

void drawPaw() {
    glColor3f(1.0, 1.0, 0.0); // yellow
    gluCylinder(quadric, 0.04, 0.04, 0.2, 32, 32);
}

// Function to assign each part with theri drawing function
void initializeDogParts() {

    // initialize Matrices
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

    // assign drawing functions
    body.f = drawBody;
    neck.f = drawNeck;
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

void updateAnimation() {
    if (isAnimating) {
        if (animationTimer < animationDuration) {
            float progress = animationTimer / (float)animationDuration;

            // For neck animation
            if (animation_id == 1) {
                if (animationType == START) {
                    neckBendAngle = originalNeckBendAngle + (80.0 * progress);
                }
                else if (animationType == FINISH) {
                    neckBendAngle = originalNeckBendAngle + (80.0 * (1.0 - progress));
                }
            }
            // For front leg animation
            else if (animation_id == 2) {
                if (animationType == START) {
                    upperLimbAngle = originalUpperLimbAngle + (-70.0f * progress); // Rotate upper limb by 30 degrees
                    lowerLimbAngle = originalLowerLimbAngle + (90.0f * progress); // Rotate lower limb by 90 degrees
                    pawAngle = originalPawAngle + (170.0f * progress); // Rotate lower limb by 90 degrees
                }
                else if (animationType == FINISH) {
                    upperLimbAngle = originalUpperLimbAngle + (-70.0f * (1.0 - progress)); // Rotate upper limb by 30 degrees
                    lowerLimbAngle = originalLowerLimbAngle + (90.0f * (1.0 - progress)); // Rotate lower limb by 90 degrees
                    pawAngle = originalPawAngle + (170.0f * (1.0 - progress)); // Rotate lower limb by 90 degrees
                }
            }
            else if (animation_id == 3) {

            }

            animationTimer += animationSpeed;
            glutPostRedisplay();
        }
        else {
            // Handle animation completion
            if (animationType == START) {
                animationType = FINISH;
                animationTimer = 0;
            }
            else {
                animationType = START;
                isAnimating = 0;
                animationTimer = 0; // Reset animation timer
                if (animation_id == 1) {
                    animationType = START;
                    neckBendAngle = originalNeckBendAngle;
                }
                else if (animation_id == 2) {
                    upperLimbAngle = originalUpperLimbAngle;
                    lowerLimbAngle = originalLowerLimbAngle;
                }
            }
            glutPostRedisplay();
        }
    }
}


void applyTransformations() {
    // Body is at the origin, so no need to move it

     // Neck
    // Translate the neck to be at the top of the body
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.25f, 1.0f); // Adjust these values as needed
    glRotatef(-25.0f + neckBendAngle, 1.0f, 0.0f, 0.0f); // Slight forward tilt + bending animation
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
    glTranslatef(-0.2f, -0.3f, 0.8f); // Left front limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.2f, -0.3f, 0.8f); // Right front limb
    glRotatef(90.0f - upperLimbAngle, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.2f, -0.3f, 0.1f); // Left back limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.2f, -0.3f, 0.1f); // Right back limb
    glRotatef(90.0f, 45.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_D.m);
    glPopMatrix();

    // Lower limbs
    // Translate each lower limb to be at the bottom of the upper limb
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f); // Adjust these values as needed
    glRotatef(-lowerLimbAngle, lowerLimbAngle, 0.0f, 1.0f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_D.m);
    glPopMatrix();

    // Paws
    // Translate each paw to be at the bottom of the lower limb

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f); // Adjust these values as needed
    glRotatef(90.0f, -180.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f); // Adjust these values as needed
    glRotatef(90.0f - pawAngle, -180.0f - pawAngle, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f); // Adjust these values as needed
    glRotatef(90.0f, -180.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f); // Adjust these values as needed
    glRotatef(90.0f, -180.0f, 0.0f, 1.0f); // Rotate outward
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_D.m);
    glPopMatrix();

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


    // Select Animation
    if (animation_id == 1)
        updateAnimation();
    else if (animation_id == 2)
        updateAnimation();

    // Apply transformations
    applyTransformations();

    // Draw the dog
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

// menu callback function
void menu(int id)
{
    if (id == 1) {
        cameraX = 0.0f;
        cameraZ = 5.0f;
        glutPostRedisplay();
    }
    else if (id == 2) {
        cameraX = 5.0f;
        cameraZ = 0.0f;
        glutPostRedisplay();
    }
    else  if (id == 3 && !isAnimating) // Ensure animation is not already in progress
        glutPostRedisplay();
    else  if (id == 4 && !isAnimating)
        glutPostRedisplay();
    else  if (id == 5 && !isAnimating)
        glutPostRedisplay();

    if (id == 3 || id == 4 || id == 5) { animation_id = id - 2;  isAnimating = 1; }
    
    if (id == 6) exit(0);

}

// create menu and options
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Camera View 1", 1);
    glutAddMenuEntry("Camera View 2", 2);
    glutAddMenuEntry("Animation 1 - Bow Neck", 3);
    glutAddMenuEntry("Animation 2 - Front Leg", 4);
    glutAddMenuEntry("Animation 3 - Standing Up", 5);
    glutAddMenuEntry("Quit", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}
