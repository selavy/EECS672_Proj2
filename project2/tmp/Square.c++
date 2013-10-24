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
  vec3* points = new vec3[NUMPOINTS];
  vec3* normals = new vec3[NUMPOINTS];

  vec3 Vertices[VERTICES] = {

  /*
   * define the square
   */
    /* front left */
    { 0.0f , 0.5f, 0.0f  },
    { 0.0f, 0.0f, 0.0f },

    /* front right */
    { 0.5f, 0.5f, 0.0f  },
    { 0.5f, 0.0f, 0.0f },

    /* back right */
    { 1.0f, 1.0f, -1.0f  },
    { 1.0f, 0.5f, -1.0f  },

    /* back left */
    { 0.0f, 1.0f, -1.0f },
    { 0.0f, 0.5f, -1.0f  }
  };

  /*
   * Following code assumes that the x- and y- coordinates
   * passed in are always parallel to the x- and
   * z- World Coordinate axes.
   */
  vec3 NormalsByFace[FACES] = {
    { 0.0f, 0.0f, 1.0f }, // front
    { -1.0f, 0.0f, 0.0f }, // left side 
    { 0.0f, 0.0f, -1.0f }, // back side
    { 1.0f, 0.0f, 0.0f }, // right side
    { 0.0f, 0.8944f, 0.447f }, // top
    { 0.0f, -0.8944f, -0.447f }, // bottom
  };

  /*
   * there is an OpenGL utility for
   * passing indice numbers, but we
   * haven't covered that yet, and I can
   * just replicate that functionality here
   * on the CPU side.
   *
   * The basic idea is to just define the
   * vertices once, then assign each vertex
   * to however many sides it is involved in
   */
  unsigned int Indices[NUMPOINTS] = 
    { 
      0, 1, 2, 3, // front
      2, 3, 5, 4, // left side
      4, 5, 7, 6, // back side
      0, 1, 7, 6, // right side
      0, 2, 4, 6, // top
      1, 3, 5, 7  // bottom
    };

  for( int i = 0; i < NUMPOINTS; ++i )
    {
      /* move the vertice and normals */
      memcpy( points[i], Vertices[ Indices[i] ], sizeof( vec3 ) );
      memcpy( normals[i], NormalsByFace[ i / FACES ], sizeof( vec3 ) );
    }

  /* move the data to the GPU */
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  /* 1 -> coordinate data; 2 -> normal vectors; */
  glGenBuffers( 2, vbo );
  /* 1. coordinate data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, NUMPOINTS * sizeof( vec3 ), points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  /* 2. normal data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, NUMPOINTS * sizeof( vec3 ), normals, GL_STATIC_DRAW );
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
  glUniform4fv( Square::ppuLoc_KD, 1, squareColor );

  glBindVertexArray( vao );
  
  for( i = 0; i < FACES /* faces of square */; ++i )
    {
      glDrawArrays( GL_TRIANGLE_STRIP, VERTICESPERFACE*i, VERTICESPERFACE*(i+1) - 1 );
    }

  /* restore the previous program */
  glUseProgram( pgm );
}
