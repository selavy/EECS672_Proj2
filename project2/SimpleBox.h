// SimpleBox.h

#ifndef SIMPLEBOX_H
#define SIMPLEBOX_H

#include "ModelView.h"
#include "ShaderIF.h"

typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[16];

class SimpleBox : public ModelView
{
public:
	SimpleBox();
	virtual ~SimpleBox();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void getWCtoECMatrix( float *);
	void getECtoLDSMatrix( float *);

	GLuint vao;
	GLuint vbo[2]; // [0] has coordinates; [1] has normals
	
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_wcNormal;
        static GLint ppuLoc_M4x4_ec_lds;
	static GLint ppuLoc_M4x4_wc_ec;
	static GLint ppuLoc_kd;

	void defineModel();
	static void fetchGLSLVariableLocations();
};

#endif
