#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAX_PARTICLES 1000
#define GRAVITY -9.81f  // global constant
# define M_PI  3.14159265358979323846
float angle = 0.0f;
float cameraX = 0.0;
float cameraZ = 20.0;

// struct to represent each particle
typedef struct {
    float position[3]; // (x, y, z)
    float velocity[3]; // (vx, vy, vz)
    float color[3];    // (r, g, b)
    int isActive; // 1 if the particle is active, 0 otherwise
} Particle;

Particle particles[MAX_PARTICLES]; // Global variable array to save all the particles

// Particles Initialazation Function
void initializeParticles() {
    srand(time(NULL)); // Seed for random number generation
    for (int i = 0; i < MAX_PARTICLES; i++) {
        /*
        particles[i].position[0] = (float)(rand() % 100) / 10.0f - 5.0f; // Random x position
        particles[i].position[1] = (float)(rand() % 100) / 10.0f + 5.0f; // Random y position but must be above the floor
        particles[i].position[2] = (float)(rand() % 100) / 10.0f - 5.0f; // Random z position
        */

        particles[i].position[0] = 0.0f;
        particles[i].position[1] = 5.0f;
        particles[i].position[2] = 0.0f;

        particles[i].velocity[0] = ((float)(rand() % 100) / 50.0f - 1.0f); // Random x velocity
        particles[i].velocity[1] = (float)(rand() % 50) / 10.0f; // Initial upward velocity
        particles[i].velocity[2] = ((float)(rand() % 100) / 50.0f - 1.0f); // Random z velocity

        // Initialize each particle with a random color
        particles[i].color[0] = (float)rand() / (float)RAND_MAX; // Red component
        particles[i].color[1] = (float)rand() / (float)RAND_MAX; // Green component
        particles[i].color[2] = (float)rand() / (float)RAND_MAX; // Blue component

        particles[i].isActive = 0; // Initially, all particles are inactive
    }
}

// Function to update particles position and velocity
void updateParticle(Particle* p, float deltaTime) {

    if (!p->isActive) return; // Check if particle is active. Update only active particles.

    float gravity[3] = { 0.0f, GRAVITY, 0.0f };

    for (int i = 0; i < 3; i++) {
        p->velocity[i] += gravity[i] * deltaTime;
        p->position[i] += p->velocity[i] * deltaTime;
    }
    // Check if the particle is below the floor
    if (p->position[1] < 0.0f) {
        p->position[1] = 0.0f;
        p->velocity[1] *= -0.8f; // Add some damping effect to the bounce

        // Apply damping to x and z velocities to simulate friction with the ground
        p->velocity[0] *= 0.9f; // Slightly reduce x velocity
        p->velocity[2] *= 0.9f; // Slightly reduce z velocity
    }
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

void updateParticles(float deltaTime) {

    for (int i = 0; i < MAX_PARTICLES; i++) {
        updateParticle(&particles[i], deltaTime);
    }
}

// Function to draw the field at y=0
void drawField() {
    // Set color to green for the field
    glColor3f(0.1, 0.1, 0.1);

    // Start drawing the field as a 15x15 square centered at (0, 0, 0)
    glBegin(GL_QUADS);
    glVertex3f(-7.5, 0.0, -7.5);
    glVertex3f(7.5, 0.0, -7.5);
    glVertex3f(7.5, 0.0, 7.5);
    glVertex3f(-7.5, 0.0, 7.5);
    glEnd();
}

#define CIRCLE_SEGMENTS 128 // More segments for a smoother circle

// Helper function to draw a circle using GL_TRIANGLE_FAN
void drawCircle(float cx, float cy, float cz, float r, const float* color) {
    glColor3fv(color); // Set the circle's color
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, cz); // Center of circle
    for (int i = 0; i <= CIRCLE_SEGMENTS; i++) {
        float angle = 2.0f * 3.1415926f * (float)i / (float)CIRCLE_SEGMENTS;
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        glVertex3f(x + cx, y + cy, cz);
    }
    glEnd();
}

void drawParticle(Particle p) {
    glPushMatrix(); // Save the current matrix

    // Translate to the particle's position
    glTranslatef(p.position[0], p.position[1], p.position[2]);

    // Set the particle's color
    glColor3fv(p.color);

    // Draw the particle as a solid sphere
    glutSolidSphere(0.05f, 16, 16); // Adjust the radius and resolution as needed

    glPopMatrix(); // Restore the matrix
}

void drawParticles() {
    //glBegin(GL_POINTS); // we are drawing points to represent the particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].isActive) { // Draw only active particles
            //glColor3fv(particles[i].color); // Set the particle's color
            //glVertex3fv(particles[i].position);
            //drawCircle(particles[i].position[0], particles[i].position[1], particles[i].position[2], 0.07f, particles[i].color);
            drawParticle(particles[i]);
        }
    }
    //glEnd();
}

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

void moveCamera(int key, int x, int y) {
    const float rotationSpeed = 2.0f;  // Speed of rotation
    float radius = 20;//find radius for rotation around center

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, 5.0, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawField();
    drawParticles();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void timer(int value) {
    activateParticles(); // Activate some particles
    updateParticles(0.02f); // Assume a fixed time step for simplicity
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Approx. 60 times per second
}

// Main function of the program
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle System");
    initGL();
    initializeParticles();
    glutSpecialFunc(moveCamera);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
