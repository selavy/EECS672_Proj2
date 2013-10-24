// SimpleBox.h

#ifndef SIMPLEBOX_H
#define SIMPLEBOX_H

#include "ModelView.h"
#include "ShaderIF.h"

class SimpleBox : public ModelView
{
public:
	SimpleBox();
	virtual ~SimpleBox();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao;
	GLuint vbo[2]; // [0] has coordinates; [1] has normals
	
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_wcPosition;
	static GLint pvaLoc_wcNormal;
	/* static GLint ppuLoc_WCECMatrix; */
	/* static GLint ppuLoc_ECLDSMatrix; */

	void defineModel();
	static void fetchGLSLVariableLocations();
};

#endif
