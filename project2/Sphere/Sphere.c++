// Sphere.c++

#include "Sphere.h"
#include <cmath>

#include <iostream>
using namespace std;

#define RGB(x) ( (x) / 255.0 )

Sphere::Sphere( 
	       int color,
	       vec3 center,           //!> location of bottom left corner of square
	       float radius
		)
  :
_color( color )
{
  _points = new vec3[ SPHERE_VERTICES ];
  _normals = new vec3[ SPHERE_VERTICES ];

  float xmin = center[0];
  float xmax = center[0] + radius;
  float ymin = center[1];
  float ymax = center[1] + radius;
  float zmin = center[2];
  float zmax = center[2] + radius;

  _limits[0] = xmin;
  _limits[1] = xmax;
  _limits[2] = ymin;
  _limits[3] = ymax;
  _limits[4] = zmin;
  _limits[5] = zmax;

  defineModel( center, radius );
} /* end Sphere::Sphere() */

Sphere::~Sphere()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end Sphere::~Sphere */

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Sphere::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, _limits, 6 * sizeof( double ) );
} /* end Sphere::getWCBoundingBox() */

void Sphere::defineModel( vec3 center, float radius )
{
  /*
  int index = 0;
  
#ifndef SQRT2
#define SQRT2 1.4142136f
#endif

  float t = radius * SQRT2;
  cout << t << endl;

  vec3 v[4] = {
    { t,      t,       t / 2.0f     },
    { t,      -1 * t,  t / 2.0f     },
    { t,      t,      -1 * t / 2.0f },
    { -1 * t, -1 * t, -1 * t / 2.0f }
  };

  divide_triangle( v[0], v[1], v[2], NUMTIMESTOSUBDIVIDE, index );
  divide_triangle( v[3], v[2], v[1], NUMTIMESTOSUBDIVIDE, index );
  divide_triangle( v[0], v[3], v[1], NUMTIMESTOSUBDIVIDE, index );
  divide_triangle( v[0], v[2], v[3], NUMTIMESTOSUBDIVIDE, index );
  */

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SPHERE_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( Sphere::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Sphere::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SPHERE_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Sphere::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( Sphere::pvaLoc_wcNormal );
} /* end Sphere::defineModel() */

void Sphere::render()
{
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  
  glUseProgram( shaderProgram );
  
  getMatrices( _limits );

  vec4 color;
  color[3] = 1.0f;

  if( _color == 1 )
    {
      color[0] = RGB( 245.0f );
      color[1] = RGB( 255.0f );
      color[2] = RGB( 250.0f );
    }
  else if( _color == 0 )
    {
      color[0] = RGB( 100 );
      color[1] = RGB( 69 );
      color[2] = RGB( 19 );
    }
  else if( _color == 2 )
    {
      color[0] = RGB( 139.0f );
      color[1] = RGB( 69.0f );
      color[2] = RGB( 19.0f );
    }
  else if( _color == 3 )
    {
      color[0] = 1.0f;
      color[1] = 0.0f;
      color[2] = 0.0f;
    }
  else
    {
      color[0] = 0.0f;
      color[1] = 0.0f;
      color[2] = 1.0f;
    }

  sendLightSource( color );

  glBindVertexArray( vao );
  glDrawArrays( GL_TRIANGLES, 0, SPHERE_VERTICES );

  glUseProgram( pgm );
} /* end Sphere::render() */

void Sphere::divide_triangle( const vec3&a, const vec3& b, const vec3& c, int count, int& index )
{
  if( count > 0 )
    {
      vec3 v1;
      vec3 v2;
      vec3 v3;
      
      cryph::AffVector av1 = cryph::AffVector(a) + cryph::AffVector(b);
      av1.normalize();

      cryph::AffVector av2 = cryph::AffVector(a) + cryph::AffVector(c);
      av2.normalize();

      cryph::AffVector av3 = cryph::AffVector(b) + cryph::AffVector(c);
      av3.normalize();

      av1.vComponents( v1 );
      av2.vComponents( v2 );
      av3.vComponents( v3 );

      divide_triangle( a, v1, v2, count - 1, index );
      divide_triangle( c, v2, v3, count - 1, index );
      divide_triangle( b, v3, v1, count - 1, index );
      divide_triangle( v1, v3, v2, count - 1, index );
    }
  else {
    tri( a, b, c, index );    
  }
} /* end Sphere::divide_triangle() */
