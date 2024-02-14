#include <GL/glut.h>

// Define the dog's parts
typedef struct DogPart {
    GLfloat m[16];
    void (*f)();
    struct DogPart* sibling;
    struct DogPart* child;
} DogPart;
