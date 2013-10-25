// Square.h

#ifndef SQUARE_H
#define SQUARE_H

#define NumVertices 36
#define NumCorners 8

#include "ModelView.h"
#include "ShaderIF.h"

typedef float mat4[16];
typedef float vec3[3];
typedef float vec4[4];

class Square : public ModelView
{
public:
  Square( bool color, vec3 corner, float width, float thickness );
	virtual ~Square();

	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();

private:
	void defineModel();
	void quad( int a, int b, int c, int d );
	void getMatrices();

	GLuint vao;
	GLuint vbo[2]; // 0: coordinate data, 1: normals

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint ppuLoc_M4x4_wc_ec;
	static GLint ppuLoc_M4x4_ec_lds;
	static GLint ppuLoc_kd;
	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_wcNormal;
	static void fetchGLSLVariableLocations();

	static vec3 _eye;
	static vec3 _center;
	static vec3 _up;
	static vec3 _eyemin;
	static vec3 _eyemax;

	int Index;

	vec3 points[NumVertices];
	vec3 normals[NumVertices];

	vec3 vertices[NumCorners];

	double limits[6];

	vec4 _kd;
	mat4 _model_view;
	mat4 _projection;
};

#endif
