// Square.h

#ifndef SQUARE_H
#define SQUARE_H

#include "NewModelView.h"

#define NumVertices 36
#define NumCorners 8

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
