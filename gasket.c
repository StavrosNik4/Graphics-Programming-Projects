#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Global variables
int numberOfPoints = 15000;
int flagRandom = 1;
int shape = 5;
int leftButtonPressed = 0;

// function to update the view based on the position of the cursor
void mouse_motion_callback_func(int x, int y) {

	if (leftButtonPressed) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluOrtho2D(-(double)x, 500.0 - (double)x, 500.0 - (double)y, -(double)y);
		//gluOrtho2D(-(double) x, 500.0 - (double) x, -(double) y, 500.0 - (double) y);
		glMatrixMode(GL_MODELVIEW);

		glutPostRedisplay(); // display

		printf("%d %d \n", x, y);
	}

}

// Callback function for mouse button events
void left_mouse_callback_func(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
			leftButtonPressed = 1;
		else
			leftButtonPressed = 0;
	}
}

void myinit(void)
{

	/* attributes */
	glEnable(GL_BLEND);
	glClearColor(1.0, 1.0, 1.0, 0.0); /* white background */
	glColor3f(1.0, 0.0, 0.0); /* draw in red */

	/* set up viewing */
	/* 500 x 500 window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(250.00, -250.00, 250.00, -250.00);
	glMatrixMode(GL_MODELVIEW);
}


void display(void)
{

	/* define a point data type */

	typedef GLfloat point2[2];

	point2 pentagon[5] = {
							{250.0, -500.0 },
							{500.0, -250.0 },
							{125.0, 0.0},
							{375.0, 0.0},
							{0.0, -275.0}
	}; // ΕΔΩ /* A pentagon */ 

	point2 hexagon[6] = {
							{250.0 / 2.0, 1000.0 / 2.0},
							{750.0 / 2.0, 1000.0 / 2.0},
							{250.0 / 2.0, 0.0},
							{750.0 / 2.0, 0.0},
							{0.0, 275.0},
							{1000.0 / 2.0, 275.0}
	}; // ΕΔΩ /* A pentagon */

	int i, j, k;
	//long rand();       /* standard random number generator */
	point2 p = { 175.0,50.0 };  /* An srbitrary initial point */

	glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

	/* computes and plots X new points */

	for (k = 0; k < numberOfPoints; k++)
	{
		j = rand() % shape; /* pick a vertex at random */	// ΕΔΩ

		/* Compute point halfway between vertex and old point */

		if (shape == 5) {
			p[0] = (p[0] + pentagon[j][0]) / 3.0; // ΕΔΩ
			p[1] = (p[1] + pentagon[j][1]) / 3.0; // ΕΔΩ
		}
		if (shape == 6) {
			p[0] = (p[0] + hexagon[j][0]) / 3.0; // ΕΔΩ
			p[1] = (p[1] + hexagon[j][1]) / 3.0; // ΕΔΩ
		}

		/* plot new point */
		if (flagRandom == 1) {
			glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX); // ΕΔΩ
		}
		else {
			glColor3f(1.0, 0.0, 0.0); /* draw in red */
		}
		glBegin(GL_POINTS);
		glVertex2fv(p);
		glEnd();

	}
	glutSwapBuffers();  // swap front and back buffers
	//glFlush(); /* clear buffers */
}

void menu(int id) //menu callback
{
	if (id == 5) exit(0);

	if (id == 1) {
		numberOfPoints = 8000;
		flagRandom = 0;
		glutPostRedisplay();
		//display();
	}

	if (id == 2) {
		numberOfPoints = 15000;
		flagRandom = 1;
		glutPostRedisplay();
		//display();
	}

	if (id == 3) {
		shape = 5;
		glutPostRedisplay();
		//display();
	}

	if (id == 4) {
		shape = 6;
		glutPostRedisplay();
		//display();
	}
}

void main(int argc, char** argv)
{

	/* Standard GLUT initialization */
	srand(time(NULL)); // seed the random number generator with current time
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); /* default, not needed */
	glutInitWindowSize(500, 500); /* 500 x 500 pixel window */
	glutInitWindowPosition(0, 0); /* place window top left on display */
	glutCreateWindow("Sierpinski Gasket"); /* window title */
	glutMouseFunc(left_mouse_callback_func); // callback registration
	glutMotionFunc(mouse_motion_callback_func); //callback registration
	glutDisplayFunc(display); /* display callback invoked when window opened */

	myinit(); /* set attributes */

	// create menu and options
	glutCreateMenu(menu);
	glutAddMenuEntry("8000 points", 1);
	glutAddMenuEntry("15000 points and random color", 2);
	glutAddMenuEntry("pentagon", 3);
	glutAddMenuEntry("hexagon", 4);
	glutAddMenuEntry("quit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // bind to right click

	glutMainLoop(); /* enter event loop */
}

