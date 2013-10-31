// Square.c++

#include "Square.h"

#include <iostream>
using namespace std;

#define RGB(x) ( (x) / 255.0 )

Square::Square( 
	       int color,
	       vec3 corner,           //!> location of bottom left corner of square
	       float width,           //!> width of square
	       float thickness        //!> thickness of square
		)
  :
_color( color )
{
  _vertices = new vec3[SQ_CORNERS];

  /*
  vec3 tmp[SQ_CORNERS] = {
    { corner[0], corner[1], corner[2] },
    { corner[0], corner[1] + thickness, corner[2] },
    { corner[0] + width, corner[1] + thickness, corner[2] },
    { corner[0] + width, corner[1], corner[2] },
    { corner[0], corner[1], corner[2] + width },
    { corner[0], corner[1] + thickness, corner[2] + width },
    { corner[0] + width, corner[1] + thickness, corner[2] + width },
    { corner[0] + width, corner[1], corner[2] + width }
  };
  */

  /* 
  float wmin = -1.0 * ( width / 2.0f );
  float wmax = -1.0f * wmin;
  float hmin = -1.0f * ( thickness / 2.0f );
  float hmax = -1.0f * hmin;
  */
 
  /*
  vec3 tmp[SQ_CORNERS] = {
    { -0.5,         -0.5,             -0.5 },
    { -0.5,         -0.5 + thickness, -0.5 },
    { -0.5 + width, -0.5 + thickness, -0.5 },
    { -0.5 + width, -0.5,             -0.5 },
    { -0.5,         -0.5,             -0.5 + width },
    { -0.5,         -0.5 + thickness, -0.5 + width },
    { -0.5 + width, -0.5 + thickness, -0.5 + width },
    { -0.5 + width, -0.5,             -0.5 + width }
  };
  */

  /*
  vec3 tmp[SQ_CORNERS] = {
    { wmin, hmin, wmin },
    { wmin, hmax, wmin },
    { wmax, hmax, wmin },
    { wmax, hmin, wmin },
    { wmin, hmin, wmax },
    { wmin, hmax, wmax },
    { wmax, hmax, wmax },
    { wmax, hmin, wmax }
  };
  */

  _points = new vec3[ SQ_VERTICES ];
  _normals = new vec3[ SQ_VERTICES ];

  float xmin = corner[0];
  float xmax = corner[0] + width;
  float ymin = corner[1];
  float ymax = corner[1] + thickness;
  float zmin = corner[2];
  float zmax = corner[2] + width;

  _limits[0] = xmin;
  _limits[1] = xmax;
  _limits[2] = ymin;
  _limits[3] = ymax;
  _limits[4] = zmin;
  _limits[5] = zmax;

  vec3 tmp[SQ_CORNERS] = {
    { xmin, ymin, zmin },
    { xmin, ymax, zmin },
    { xmax, ymax, zmin },
    { xmax, ymin, zmin },
    { xmin, ymin, zmax },
    { xmin, ymax, zmax },
    { xmax, ymax, zmax },
    { xmax, ymin, zmax }
  };

  memcpy( _vertices, tmp, SQ_CORNERS * sizeof( vec3 ) );

  defineModel();
} /* end Square::Square() */

Square::~Square()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  delete [] _points;
  delete [] _normals;
  delete [] _vertices;
} /* end Square::~Square */

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Square::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, _limits, 6 * sizeof( double ) );
} /* end Square::getWCBoundingBox() */

void Square::defineModel()
{
  int index = 0;

  /*
  quad( _V(1), _V(0), _V(3), _V(2), index );
  quad( _V(2), _V(3), _V(7), _V(6), index );
  quad( _V(3), _V(0), _V(4), _V(7), index );
  quad( _V(6), _V(5), _V(1), _V(2), index );
  quad( _V(4), _V(5), _V(6), _V(7), index );
  quad( _V(5), _V(4), _V(0), _V(1), index );
  */
  quad( 1, 0, 3, 2, index );
  quad( 3, 0, 4, 7, index );
  quad( 2, 3, 7, 6, index );
  quad( 5, 4, 0, 1, index );
  quad( 6, 5, 1, 2, index );
  quad( 4, 5, 6, 7, index );

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SQ_VERTICES, _points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * SQ_VERTICES, _normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcNormal );
} /* end Square::defineModel() */

void Square::render()
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
  glDrawArrays( GL_TRIANGLES, 0, SQ_VERTICES );

  glUseProgram( pgm );
} /* end Square::render() */
