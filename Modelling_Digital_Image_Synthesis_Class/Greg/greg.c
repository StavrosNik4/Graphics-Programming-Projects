#include "greg.h"

// Camera Global Variables
float cameraAngle = 0.0f;
float cameraX = 5.0f;
float cameraZ = 0.0f;

// Animation Global Variables
int animation_id = 0;
int isAnimating = 0;
int reseting = 0;

int messageFlag = 0;

float animationSpeed = 0.95f;
float animationTimer = 0.0;
float animationDuration = 100.0;

// neck animation related 
float neckBendAngle = 0.0f;

// leg animation related
float upperLimbAngle = 0.0f;
float lowerLimbAngle = 0.0f;
float pawAngle = 0.0f;

// body animation related
float bodyY = 0.0f;
float bodyAngle = 0.0f;
float frontUpperLimbAngle = 0.0f;
float frontLowerLimbAngle = 0.0f;
float frontPawAngle = 0.0f;
float backUpperLimbAngle = 0.0f;
float backLowerLimbAngle = 0.0f;
float backPawAngle = 0.0f;

// Animation state types
enum AnimationStateType { START, FINISH };
enum AnimationStateType animationStateType = START;

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

// Function to assign each part with their drawing function
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
                if (animationStateType == START) {
                    neckBendAngle = (80.0 * progress);
                }
                else if (animationStateType == FINISH && reseting == 1) {
                    neckBendAngle = (80.0 * (1.0 - progress));
                }
            }
            // For front leg animation
            else if (animation_id == 2) {
                if (animationStateType == START) {
                    upperLimbAngle = (-70.0f * progress); // Rotate upper limb by 70 degrees
                    lowerLimbAngle = (90.0f * progress); // Rotate lower limb by 90 degrees
                    pawAngle =  (170.0f * progress); // Rotate lower limb by 170 degrees
                }
                else if (animationStateType == FINISH && reseting == 1) {
                    upperLimbAngle = (-70.0f * (1.0 - progress)); // Rotate upper limb by 70 degrees
                    lowerLimbAngle = + (90.0f * (1.0 - progress)); // Rotate lower limb by 90 degrees
                    pawAngle = (170.0f * (1.0 - progress)); // Rotate lower limb by 170 degrees
                }
            }
            // Back Legs animation
            else if (animation_id == 3) {
                if (animationStateType == START) {
                    bodyY = 0.7f * progress;
                    bodyAngle = (80.0f * progress);
                    neckBendAngle = (60.0 * progress);

                    frontUpperLimbAngle = (-80.0f * progress);
                    frontLowerLimbAngle = (80.0f * progress);
                    frontPawAngle = (170.0f * progress);

                    backUpperLimbAngle = (-120.0f * progress);
                    backLowerLimbAngle = (130.0f * progress);
                    backPawAngle = (90.0f * progress);
                }
                else if (animationStateType == FINISH && reseting == 1) {
                    bodyY = 0.7f * (1.0 - progress);
                    bodyAngle = (90.0f * (1.0 - progress));
                    neckBendAngle = (60.0 * (1.0 - progress));
                    
                    frontUpperLimbAngle = (-80.0f * (1.0 - progress));
                    frontLowerLimbAngle = (80.0f * (1.0 - progress));
                    frontPawAngle = (170.0f * (1.0 - progress));

                    backUpperLimbAngle = (-120.0f * (1.0 - progress));
                    backLowerLimbAngle = (130.0f * (1.0 - progress));
                    backPawAngle = (90.0f * (1.0 - progress));
                }
            }

            animationTimer += animationSpeed;
            glutPostRedisplay();
        }
        else {
            // Handle animation completion
            if (animationStateType == START)
                animationStateType = FINISH;
            else {
                animationStateType = START;
                reseting = 0;
                animation_id = 0;
            }
            animationTimer = 0;
            isAnimating = 0;
            glutPostRedisplay();
        }
    }
}

// Function to apply the transformations to each part
void applyTransformations() {
    // Body 
    glPushMatrix();
    glLoadIdentity();
    if (animation_id == 3) {
        glTranslatef(0.0, -bodyY, 0.0f);
        glRotatef(bodyAngle, -bodyAngle, 0.0f, 0.0f);
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, body.m);
    glPopMatrix();

    // Neck
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.25f, 1.0f);
    if(animation_id == 1 || animation_id == 3)
        glRotatef(-25.0f + neckBendAngle, 1.0f, 0.0f, 0.0f);
    else
        glRotatef(-25.0f, 1.0f, 0.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, neck.m);
    glPopMatrix();

    // Head
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.1f, 0.4f); // Adjust these values as needed
    glGetFloatv(GL_MODELVIEW_MATRIX, head.m);
    glPopMatrix();

    // Upper limbs
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.2f, -0.3f, 0.8f);
    if (animation_id == 3)
        glRotatef(90.0f - frontUpperLimbAngle, 45.0f, 0.0f, 1.0f);
    else
        glRotatef(90.0f, 45.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.2f, -0.3f, 0.8f);
    if (animation_id == 3)
        glRotatef(90.0f - frontUpperLimbAngle, 45.0f, 0.0f, 1.0f);
    else if (animation_id == 2)
        glRotatef(90.0f - upperLimbAngle, 45.0f, 0.0f, 1.0f);
    else
        glRotatef(90.0f, 45.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.2f, -0.3f, 0.1f);
    if (animation_id == 3)
        glRotatef(90.0f - backUpperLimbAngle, 45.0f, 0.0f, 1.0f);
    else
        glRotatef(90.0f, 45.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.2f, -0.3f, 0.1f);
    if (animation_id == 3)
        glRotatef(90.0f - backUpperLimbAngle, 45.0f, 0.0f, 1.0f);
    else
        glRotatef(90.0f, 45.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, upper_limb_D.m);
    glPopMatrix();

    // Lower limbs
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f);
    if(animation_id == 3)
        glRotatef(-frontLowerLimbAngle, frontLowerLimbAngle, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f);
    if (animation_id == 3)
        glRotatef(-frontLowerLimbAngle, frontLowerLimbAngle, 0.0f, 1.0f);
    else if (animation_id == 2)
        glRotatef(-lowerLimbAngle, lowerLimbAngle, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f); 
    if (animation_id == 3)
        glRotatef(-backLowerLimbAngle, backLowerLimbAngle, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.4f); 
    if (animation_id == 3)
        glRotatef(-backLowerLimbAngle, backLowerLimbAngle, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lower_limb_D.m);
    glPopMatrix();

    // Paws
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f);
    if (animation_id == 3)
        glRotatef(90.0f - frontPawAngle, -180.0f - frontPawAngle, 0.0f, 1.0f);
    else
        glRotatef(90.0f, -180.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_A.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f);
    if (animation_id == 3)
        glRotatef(90.0f - frontPawAngle, -180.0f - frontPawAngle, 0.0f, 1.0f);
    else if (animation_id == 2)
        glRotatef(90.0f - pawAngle, -180.0f - pawAngle, 0.0f, 1.0f);
    else
        glRotatef(90.0f, -180.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_B.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f);
    if(animation_id == 3)
        glRotatef(90.0f - backPawAngle, -180.0f, 0.0f, 1.0f);
    else
        glRotatef(90.0f, -180.0f, 0.0f, 1.0f); 
    glGetFloatv(GL_MODELVIEW_MATRIX, paw_C.m);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.3f); 
    if (animation_id == 3)
        glRotatef(90.0f - backPawAngle, -180.0f, 0.0f, 1.0f);
    else
        glRotatef(90.0f, -180.0f, 0.0f, 1.0f);
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

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    float x_start = x;
    int line_height = 18; // Adjust line height as needed

    for (c = string; *c != '\0'; c++) {
        // Wrap text if it exceeds the window width
        if (*c == '\n' || x + glutBitmapWidth(font, *c) > glutGet(GLUT_WINDOW_WIDTH)) {
            y -= line_height; // Move to the next line
            x = x_start;      // Reset x position
            if (*c == '\n')   // Skip newline characters
                continue;
        }

        glRasterPos2f(x, y);
        glutBitmapCharacter(font, *c);
        x += glutBitmapWidth(font, *c); // Move x for the next character
    }
}

void drawMessage() {
    // Set the color to white for the text
    glColor3f(1.0f, 1.0f, 1.0f);

    // Move back to a 2D view to draw the text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render the text strings
    renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_9_BY_15, 
        "Warning! You need to reset the position of the model to perform another animation.");

    // Restore the previous projection and modelview matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update gluLookAt with dynamic cameraX and cameraZ values
    gluLookAt(cameraX, 0.0, cameraZ,  // eye position
        0.0, 0.0, 0.0,           // center position (looking at)
        0.0, 1.0, 0.0);          // up vector

    if(messageFlag)
        drawMessage();

    // Select Animation
    if (animation_id == 1)
        updateAnimation();
    else if (animation_id == 2)
        updateAnimation();
    else if (animation_id == 3)
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

    if ((id == 3 || id == 4 || id == 5) && !isAnimating && animationStateType == START) {
        animation_id = id - 2;  
        isAnimating = 1; 
        messageFlag = 1;
        glutPostRedisplay(); 
    }
    
    
    if (id == 6 && !isAnimating && animationStateType == FINISH) {
        reseting = 1;
        isAnimating = 1;
        messageFlag = 0;
        glutPostRedisplay();
    }

    if (id == 7) exit(0);

}

// create menu and options
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Camera View 1", 1);
    glutAddMenuEntry("Camera View 2", 2);
    glutAddMenuEntry("Animation 1 - Bow Neck", 3);
    glutAddMenuEntry("Animation 2 - Front Leg", 4);
    glutAddMenuEntry("Animation 3 - Standing Up", 5);
    glutAddMenuEntry("Reset Position", 6);
    glutAddMenuEntry("Quit", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click
}
