// project2.c++: Starter for EECS 672 Project 2

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Controller.h"
#include "SimpleBox.h"

int main(int argc, char* argv[])
{
	// One-time initialization of the glut
	glutInit(&argc, argv);

	Controller c("Checkers", GLUT_DEPTH | GLUT_DOUBLE);

	// create your scene, adding things to the Controller....
	c.addModel( new SimpleBox );

	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Off to the glut event handling loop:
	glutMainLoop();

	return 0;
}
