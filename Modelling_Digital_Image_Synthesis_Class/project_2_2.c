#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAX_PARTICLES 1000
#define GRAVITY -9.81f

typedef struct {
    float position[3]; // Θέση σωματιδίου (x, y, z)
    float velocity[3]; // Ταχύτητα σωματιδίου (vx, vy, vz)
    int isActive; // 1 if the particle is active, 0 otherwise
    float color[3];    // Color of the particle (r, g, b)
} Particle;

Particle particles[MAX_PARTICLES];

void updateParticle(Particle* p, float deltaTime) {
    if (!p->isActive) return; // Update only active particles
    float gravity[3] = { 0.0f, GRAVITY, 0.0f };
    for (int i = 0; i < 3; i++) {
        p->velocity[i] += gravity[i] * deltaTime;
        p->position[i] += p->velocity[i] * deltaTime;
    }
    if (p->position[1] < 0.0f) {
        p->position[1] = 0.0f;
        p->velocity[1] *= -0.8f; // Add some damping effect to the bounce
    }
}

void initializeParticles() {
    srand(time(NULL)); // Seed for random number generation
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].position[0] = (float)(rand() % 100) / 10.0f - 5.0f; // Random x position
        particles[i].position[1] = (float)(rand() % 100) / 10.0f; // Random y position above the floor
        particles[i].position[2] = (float)(rand() % 100) / 10.0f - 5.0f; // Random z position

        particles[i].velocity[0] = 0.0f;
        particles[i].velocity[1] = (float)(rand() % 50) / 10.0f; // Initial upward velocity
        particles[i].velocity[2] = 0.0f;

        particles[i].isActive = 0; // Initially, all particles are inactive

        // Initialize each particle with a random color
        particles[i].color[0] = (float)rand() / (float)RAND_MAX; // Red component
        particles[i].color[1] = (float)rand() / (float)RAND_MAX; // Green component
        particles[i].color[2] = (float)rand() / (float)RAND_MAX; // Blue component
    }
}

// Global variable to keep track of the last activated particle
int lastActivatedParticle = 0;


void activateParticles() {
    // Define how many particles to activate per frame
    int particlesToActivate = 5;

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

void drawParticles() {
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].isActive) { // Draw only active particles
            glColor3fv(particles[i].color); // Set the particle's color
            glVertex3fv(particles[i].position);
        }
    }
    glEnd();
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(5.0f);

    // Set up a basic perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawParticles();
    glutSwapBuffers();
}

void timer(int value) {
    activateParticles(); // Activate some particles
    updateParticles(0.02f); // Assume a fixed time step for simplicity
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Approx. 60 times per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle System");
    initGL();
    initializeParticles();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
