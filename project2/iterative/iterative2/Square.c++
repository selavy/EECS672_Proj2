// Square.c++

#include "Square.h"
#include <cstring>

#ifndef RGB
#define RGB( x ) ( x / 0xFF )
#endif

#define __DEBUG__

#define FACES 6
#define VERTICESPERFACE 4
#define VERTICES 8
#define NUMPOINTS ( VERTICESPERFACE * VERTICES )

#define ALPHAVALUE 1

#ifdef __DEBUG__
#include <iostream>
#endif

typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[4*4];

Square::Square()
{
  defineModel();
}

Square::~Square()
{
  glDeleteBuffers( 2, vbo );
  glDeleteVertexArrays( 1, &vao );
}

void Square::defineModel()
{
   /* 6 (faces) * 4 (vertices / face) = 24 vertices */


  vec3* points = new vec3[24];
  vec3* normals = new vec3[24];

  short i;

  /* fill in the data for a cube */

  /* front */
  points[0][0] = -0.1;
  points[0][1] = -0.1;
  points[0][2] = 0.1;

  points[1][0] = 0.1;
  points[1][1] = -0.1;
  points[1][2] = 0.1;

  points[2][0] = 0.1;
  points[2][1] = 0.1;
  points[2][2] = 0.1;

  points[3][0] = -0.1;
  points[3][1] = 0.1;
  points[3][2] = 0.1;

  /* back */
  points[4][0] = -0.1;
  points[4][1] = -0.1;
  points[4][2] = -0.1;

  points[5][0] = 0.1;
  points[5][1] = -0.1;
  points[5][2] = -0.1;

  points[6][0] = 0.1;
  points[6][1] = 0.1;
  points[6][2] = -0.1;

  points[7][0] = -0.1;
  points[7][1] = 0.1;
  points[7][2] = -0.1;

  /* left */
  points[8][0] = -0.1;
  points[8][1] = -0.1;
  points[8][2] = 0.1;

  points[9][0] = -0.1;
  points[9][1] = -0.1;
  points[9][2] = -0.1;

  points[10][0] = -0.1;
  points[10][1] = 0.1;
  points[10][2] = 0.1;

  points[11][0] = -0.1;
  points[11][1] = 0.1;
  points[11][2] = -0.1;

  /* right */
  points[12][0] = 0.1;
  points[12][1] = -0.1;
  points[12][2] = 0.1;

  points[13][0] = 0.1;
  points[13][1] = 0.1;
  points[13][2] = 0.1;

  points[14][0] = 0.1;
  points[14][1] = -0.1;
  points[14][2] = -0.1;

  points[15][0] = 0.1;
  points[15][1] = 0.1;
  points[15][2] = -0.1;

  /* top */
  points[16][0] = 0.1;
  points[16][1] = 0.1;
  points[16][2] = 0.1;

  points[17][0] = 0.1;
  points[17][1] = 0.1;
  points[17][2] = -0.1;

  points[18][0] = -0.1;
  points[18][1] = 0.1;
  points[18][2] = 0.1;

  points[19][0] = -0.1;
  points[19][1] = 0.1;
  points[19][2] = -0.1;

  /* bottom */
  points[20][0] = -0.1;
  points[20][1] = -0.1;
  points[20][2] = -0.1;

  points[21][0] = -0.1;
  points[21][1] = -0.1;
  points[21][2] = 0.1;

  points[22][0] = 0.1;
  points[22][1] = -0.1;
  points[22][2] = -0.1;

  points[23][0] = 0.1;
  points[23][1] = -0.1;
  points[23][2] = 0.1;

  for( i = 0; i < 4; ++i )
    {
      normals[i][0] = 0;
      normals[i][1] = 0;
      normals[i][2] = -1;
    }

  for( i = 4; i < 8; ++i )
    {
      normals[i][0] = 0;
      normals[i][1] = 0;
      normals[i][2] = 1;
    }

  for( i = 8; i < 12; ++i )
    {
      normals[i][0] = 1;
      normals[i][1] = 0;
      normals[i][2] = 0;
    }

  for( i = 12; i < 16; ++i )
    {
      normals[i][0] = -1;
      normals[i][1] = 0;
      normals[i][2] = 0;
    }

  for( i = 16; i < 20; ++i )
    {
      normals[i][0] = 0;
      normals[i][1] = -1;
      normals[i][2] = 0;
    }

  for( i = 20; i < 24; ++i )
    {
      normals[i][0] = 0;
      normals[i][1] = 1;
      normals[i][2] = 0;
    }

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 2, vbo );
  /* 1. coordinate data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, 24 * sizeof( vec3 ), points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  /* 2. normal data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, 24 * sizeof( vec3 ), normals, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcNormal );

  delete [] points;
  delete [] normals;
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Square::getWCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1; // xmin
	xyzLimits[1] = 1;  // xmax
	xyzLimits[2] = -1; // ymin
	xyzLimits[3] = 1; // ymax
	xyzLimits[4] = -1; // zmin
	xyzLimits[5] = 1;  // zmax
}

void Square::render()
{
  GLint pgm;
  short i;

  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  glUseProgram( shaderProgram );

  BoardMV::sendMatrices();

  /*
   * Get the square color
   * if _color true, then lighter color;
   * else darker color
   */
  //  vec4 squareColor = { RGB(238), RGB(233), RGB(233), RGB(0) };
  vec4 squareColor = { 1.0, 0.0, 0.0, 1.0 };

  /* 
   * send the square color to the GPU
   */
  //glUniform4fv( BoardMV::ppuLoc_kd, 1, squareColor );

  /*
  glBindVertexArray( vao );
  
  for( i = 0; i < FACES; ++i )
    {
      glDrawArrays( GL_TRIANGLE_STRIP, VERTICESPERFACE*i, VERTICESPERFACE*(i+1) - 1 );
    }
  */

  glBindVertexArray( vao );
  
  for( i = 0; i < 6 /* faces of square */; ++i )
    {
      glDrawArrays( GL_TRIANGLE_STRIP, 4*i, 4*(i+1) - 1 );
    }

  /* restore the previous program */
  glUseProgram( pgm );
}
