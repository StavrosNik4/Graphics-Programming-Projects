#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// Constants
#define MAX_PARTICLES 1000
#define GRAVITY -9.81f  
#define M_PI  3.14159265358979323846

// Camera global variables
float angle = 0.0f;
float cameraX = 0.0;
float cameraZ = 20.0;

// Global variable for the color drawing mode
int colorOption = 1;

// struct to represent each particle
typedef struct {
    float position[3]; // (x, y, z)
    float velocity[3]; // (vx, vy, vz)
    float color[3];    // (r, g, b)
    int isActive; // 1 if the particle is active, 0 otherwise
} Particle;

// Global variable array to save all the particles
Particle particles[MAX_PARTICLES];

void changeParticleColors() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (colorOption == 1) { // random
            // Initialize each particle with a random color
            particles[i].color[0] = (float)rand() / (float)RAND_MAX; // Red component
            particles[i].color[1] = (float)rand() / (float)RAND_MAX; // Green component
            particles[i].color[2] = (float)rand() / (float)RAND_MAX; // Blue component
        }
        else if (colorOption == 2) { // pink
            particles[i].color[0] = 1.0f; // Red component
            particles[i].color[1] = 0.75f; // Green component
            particles[i].color[2] = 0.8f; // Blue component
        }
        else if (colorOption == 3) { // green
            particles[i].color[0] = 0.0f; // Red component
            particles[i].color[1] = 1.0f; // Green component
            particles[i].color[2] = 0.0f; // Blue component
        }
        else if (colorOption == 4) { // yellow
            particles[i].color[0] = 1.0f; // Red component
            particles[i].color[1] = 1.0f; // Green component
            particles[i].color[2] = 0.0f; // Blue component
        }
    }
}

// Particles Initialazation Function
void initializeParticles() {
    srand(time(NULL)); // Seed for random number generation
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].position[0] = 0.0f;
        particles[i].position[1] = 5.0f;
        particles[i].position[2] = 0.0f;

        particles[i].velocity[0] = ((float)(rand() % 100) / 50.0f - 1.0f); // Random x velocity
        particles[i].velocity[1] = (float)(rand() % 50) / 10.0f; // Initial upward velocity
        particles[i].velocity[2] = ((float)(rand() % 100) / 50.0f - 1.0f); // Random z 

        particles[i].isActive = 0; // Initially, all particles are inactive
    }

    changeParticleColors();
}

// Global variable to keep track of the last activated particle
int lastActivatedParticle = 0;

void activateParticles() {

    int particlesToActivate = 2; // How many particles to activate per frame

    for (int i = 0; i < particlesToActivate; i++) {
        if (lastActivatedParticle < MAX_PARTICLES) {
            particles[lastActivatedParticle].isActive = 1;
            lastActivatedParticle++;
        }
    }
}

// Function to update a particle's position and velocity
void updateParticle(Particle* p, float deltaTime) {

    // Check if particle is active. Update only active particles.
    if (!p->isActive) return;

    float gravity[3] = { 0.0f, GRAVITY, 0.0f };

    for (int i = 0; i < 3; i++) {
        p->velocity[i] += gravity[i] * deltaTime;
        p->position[i] += p->velocity[i] * deltaTime;
    }
    // Check if the particle is below the floor, below y = 0
    if (p->position[1] < 0.0f) {
        p->position[1] = 0.0f;
        p->velocity[1] *= -0.8f; // Add some damping effect to the bounce

        // Apply damping to x and z velocities to simulate friction with the ground
        p->velocity[0] *= 0.9f; // Slightly reduce x velocity
        p->velocity[2] *= 0.9f; // Slightly reduce z velocity
    }
}

void updateParticles(float deltaTime) {
    for (int i = 0; i < MAX_PARTICLES; i++)
        updateParticle(&particles[i], deltaTime);
}

// Function to draw the field at y=0
void drawField() {
    // Set color to green for the field
    glColor3f(0.1, 0.1, 0.1);

    // Start drawing the field as a 15x15 square centered at (0, -0.05, 0). 
    // It's slighty down to the y axis cause of the sphere radius.
    glBegin(GL_QUADS);
    glVertex3f(-7.5, -0.05, -7.5);
    glVertex3f(7.5, -0.05, -7.5);
    glVertex3f(7.5, -0.05, 7.5);
    glVertex3f(-7.5, -0.05, 7.5);
    glEnd();
}

void drawParticle(Particle p) {
    glPushMatrix(); // Save the current matrix

    // Translate to the particle's position
    glTranslatef(p.position[0], p.position[1], p.position[2]);

    // Set the particle's color
    glColor3fv(p.color);

    // Draw the particle as a solid sphere
    glutSolidSphere(0.05f, 16, 16); // radius and resolution for the sphere

    glPopMatrix(); // Restore the matrix
}

void drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++)
        if (particles[i].isActive) // Draw only active particles
            drawParticle(particles[i]);
}

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

void drawText() {
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
    renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_9_BY_15, "Press 'R' to restart.");
    renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 35, GLUT_BITMAP_9_BY_15, "Press '1-4' to change colors.");
    renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 50, GLUT_BITMAP_9_BY_15, "Use left and right key to inspect.");

    // Restore the previous projection and modelview matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// OpenGL Initialazation Function
void initGL() {

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(5.0f);

    // Set up a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Function for the movement of the camera
void moveCamera(int key, int x, int y) {
    const float rotationSpeed = 2.0f;  // Speed of rotation
    float radius = 20; //find radius for rotation around center

    switch (key) {
    case GLUT_KEY_RIGHT:
        angle += rotationSpeed;  // Increase the angle for clockwise rotation
        cameraX = radius * sin(angle * M_PI / 180.0);//angle * M_PI / 180.0 to convert angle into radians
        cameraZ = radius * cos(angle * M_PI / 180.0);
        break;
    case GLUT_KEY_LEFT:
        angle -= rotationSpeed;  // Decrease the angle for counterclockwise rotation
        cameraX = radius * sin(angle * M_PI / 180.0);
        cameraZ = radius * cos(angle * M_PI / 180.0);
        break;
    }

    glutPostRedisplay();  // Request a redraw of the scene
}

// Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, 5.0, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawText();

    drawField();
    drawParticles();
    glutSwapBuffers();
}

// Function to handle the window reshape properly
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void timer(int value) {
    activateParticles(); // Activate some particles
    updateParticles(0.02f); // Assume a fixed time step for simplicity
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Approx. 60 times per second
}

// Keyboard callback functions
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r': // If 'R' or 'r' is pressed, restart the particle system
    case 'R':
        initializeParticles(); // Reinitialize the particles
        lastActivatedParticle = 0; // Reset the index of the last activated particle
        break;
    case '1':
        colorOption = 1;
        changeParticleColors();
        break;
    case '2':
        colorOption = 2;
        changeParticleColors();
        break;
    case '3':
        colorOption = 3;
        changeParticleColors();
        break;
    case '4':
        colorOption = 4;
        changeParticleColors();
        break;
    default:
        break;
    }
    glutPostRedisplay(); // Redraw the scene with the updated particles
}

// Main function of the program
int main(int argc, char** argv) {

    // glut initialazation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle System");

    // OpenGL initialazation
    initGL();

    // begin
    initializeParticles();

    // Registering callback functions
    glutSpecialFunc(moveCamera);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}