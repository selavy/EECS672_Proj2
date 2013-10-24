// project2.c++: Starter for EECS 672 Project 2

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Controller.h"
#include "Square.h"

int main(int argc, char* argv[])
{
	// One-time initialization of the glut
	glutInit(&argc, argv);

	Controller c("GIVE ME A NICE TITLE", GLUT_DEPTH | GLUT_DOUBLE);

	// create your scene, adding things to the Controller....
	c.addModel( new Square(true, 0.0, 0.0, 0.2, 0.5) );
	c.addModel( new Square( false, 0.5, 0.0, 0.2, 0.5 ) );

	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Off to the glut event handling loop:
	glutMainLoop();

	return 0;
}