// Square.h

#ifndef SQUARE_H
#define SQUARE_H

#include "BoardMV.h"
#include "ShaderIF.h"

class Square : public BoardMV
{
public:
  Square( bool color, float xcoord, float ycoord, float thickness, float width );
	virtual ~Square();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	GLuint vao;
	GLuint vbo[3]; // [0] has coordinates; [1] has normals; [2] colors
	
	void defineModel();

	bool _color;
	float _x;
	float _y;
	float _thickness;
	float _width;
};

#endif
