// project2.c++: Starter for EECS 672 Project 2

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Controller.h"
#include "Prism.h"
#include "ShaderIF.h"

#define RED true
#define BLACK false
#define FLIP true

int main(int argc, char* argv[])
{
	// One-time initialization of the glut
	glutInit(&argc, argv);

	Controller c("Your move", GLUT_DEPTH | GLUT_DOUBLE);

	float width = 1.0f;
	float thickness = 0.2f;
	vec3 coords = { -8.0f, 0.0f, -8.0f };
	bool color = BLACK;
	
	// create your scene, adding things to the Controller....
	
	/* Create the board */
	for( int i = 0; i < 8; ++i )
	  {
	    for( int j = 0; j < 8; ++j )
	      {
		c.addModel( new Prism( color, coords, width, thickness ) );
		coords[0] += width;
		color ^= FLIP;
	      }
	    color ^= FLIP;
	    coords[0] = -8.0f;
	    coords[2] += width;
	  }


	coords[0] = -7.9f;
	coords[1] =  0.0f;
	coords[2] = -7.9f;
	c.addModel( new Prism( color, coords, 0.3, 5.0 ) );

	coords[0] = 7.9f;
	coords[1] = 0.0f;
	coords[2] = 7.9f;

	c.addModel( new Prism( color, coords, 0.3, 5.0 ) );
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Off to the glut event handling loop:
	glutMainLoop();

	return 0;
}
