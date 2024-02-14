#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

// Structure to represent each dog part
typedef struct DogPart {
    GLfloat m[16];
    void (*f)();
    struct DogPart* sibling;
    struct DogPart* child;
} DogPart;

// Initialize the dog parts
DogPart body, neck, head, upper_limb_A, lower_limb_A, paw_A, upper_limb_B, lower_limb_B, paw_B, upper_limb_C, lower_limb_C, paw_C, upper_limb_D, lower_limb_D, paw_D;

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

void display() {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    // Swap buffers to display the frame
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("What the dog doin?");



    // Set the display function
    glutDisplayFunc(display);

    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}
