// Sphere.h

#ifndef SPHERE_H
#define SPHERE_H

#include "GeneralMV.h"
#include "AffVector.h"

#define NUMTIMESTOSUBDIVIDE 5
#define NUMTRIANGLES 4096
#define SPHERE_VERTICES (3 * NUMTRIANGLES)

#define _V( x ) _vertices[ (x) ]

class Sphere : public GeneralMV
{
public:
  Sphere( int color, vec3 center, float radius );
	virtual ~Sphere();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox( double* xyzLimitsF ) const;
	void defineModel( vec3 center, float radius );
	void render();

private:
	void divide_triangle( const vec3& a, const vec3& b, const vec3& c, int count, int& index );

	GLuint vao;
	GLuint vbo[2]; //!> 0: coordinate data, 1: normals
	
	double _limits[6];
	int _color;

};

#endif
