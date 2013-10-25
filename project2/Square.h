// Square.h

#ifndef SQUARE_H
#define SQUARE_H

#define NumVertices 36
#define NumCorners 8

#include "NewModelView.h"

typedef float mat4[16];
typedef float vec3[3];
typedef float vec4[4];

class Square : public NewModelView
{
public:
  Square( bool color, vec3 corner, float width, float thickness );
	virtual ~Square();

	void render();

private:
	void defineModel();
	void quad( int a, int b, int c, int d );

	GLuint vao;
	GLuint vbo[2]; // 0: coordinate data, 1: normals

	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_wcNormal;
	static void fetchGLSLVariableLocations();

	int Index;

	vec3 points[NumVertices];
	vec3 normals[NumVertices];

	vec3 vertices[NumCorners];
};

#endif
