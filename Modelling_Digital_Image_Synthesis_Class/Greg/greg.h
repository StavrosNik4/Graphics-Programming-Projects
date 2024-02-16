#ifndef DOG_H
#define DOG_H

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

// Camera global variables
extern float cameraAngle;
extern float cameraX;
extern float cameraZ;

// Define the dog parts
extern DogPart body, neck, head,
upper_limb_A, lower_limb_A, paw_A,
upper_limb_B, lower_limb_B, paw_B,
upper_limb_C, lower_limb_C, paw_C,
upper_limb_D, lower_limb_D, paw_D;

// Function prototypes
void createTree();
void setIdentityMatrix(GLfloat* matrix);
void initQuadric();
void drawBody();
void drawNeck();
void drawHead();
void drawUpperLimb();
void drawLowerLimb();
void drawPaw();
void initializeDogParts();
void applyTransformations();
void drawDogPart(DogPart* part);
void display();
void setupProjection();
void reshape(int width, int height);
void menu(int id);
void createMenu();

#endif // DOG_H
