#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{

	int numberOfPoints = 8000;

	/* define a point data type */

	typedef GLfloat point2[2];

	GLdouble r = 400.0;
	point2 vertices[5] = {
							{500.0, 1000.0},
							{250.0, 0.0},
							{750.0, 0.0},
							{0.0, 2.0 * 275.0},
							{1000.0, 2.0 * 275.0}
	}; // ΕΔΩ /* A pentagon */ 

	int i, j, k;
	//long rand();       /* standard random number generator */
	point2 p = { 175.0,50.0 };  /* An srbitrary initial point */

	glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

	/* computes and plots 8000 new points */

	for (k = 0; k < numberOfPoints; k++)
	{
		j = rand() % 5; /* pick a vertex at random */	// ΕΔΩ

		/* Compute point halfway between vertex and old point */

		p[0] = (p[0] + vertices[j][0]) / 3.0; // ΕΔΩ
		p[1] = (p[1] + vertices[j][1]) / 3.0; // ΕΔΩ

		/* plot new point */
		glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX); // ΕΔΩ
		glBegin(GL_POINTS);
		glVertex2fv(p);
		glEnd();

	}
	glFlush(); /* clear buffers */
}

void main(int argc, char** argv)
{

	/* Standard GLUT initialization */
	srand(time(NULL)); // seed the random number generator with current time
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); /* default, not needed */
	glutInitWindowSize(500, 500); /* 500 x 500 pixel window */
	glutInitWindowPosition(0, 0); /* place window top left on display */
	glutCreateWindow("Sierpinski Gasket"); /* window title */
	glutDisplayFunc(display); /* display callback invoked when window opened */

	myinit(); /* set attributes */

	glutMainLoop(); /* enter event loop */
}

