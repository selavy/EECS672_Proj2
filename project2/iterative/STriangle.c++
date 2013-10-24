// STriangle.c++

#include "STriangle.h"
#include<cstring>

#define __DEBUG__

#define FACES 4
#define VERTICESPERFACE 3
#define VERTICES ( FACES * VERTICESPERFACE )

#ifdef __DEBUG__
#include <iostream>
#endif

typedef float vec3[3];

/* static */ ShaderIF * STriangle::shaderIF = NULL;
/* static */ int        STriangle::numInstances = 0;
/* static */ GLuint     STriangle::shaderProgram = 0;
/* static */ GLint      STriangle::pvaLoc_wcPosition = -1;
/* static */ GLint      STriangle::pvaLoc_wcNormal = -1;

STriangle::STriangle()
{
#ifdef __DEBUG__
  std::cout << "Creating instance #" << numInstances + 1 << " of STriangle..." << std::endl;
#endif

  /* if the first time an instance of this class is generated, then create a common shader program */
  if( STriangle::shaderProgram == 0 )
    {
      STriangle::shaderIF = new ShaderIF( "STriangle.vsh", "STriangle.fsh" );
      STriangle::shaderProgram = shaderIF->getShaderPgmID();
      fetchGLSLVariableLocations();
    }

  defineModel();
  STriangle::numInstances++;
}

STriangle::~STriangle()
{
  glDeleteBuffers( 2, vbo );
  glDeleteVertexArrays( 1, &vao );

  if( (--STriangle::numInstances) == 0 )
    {
      STriangle::shaderIF->destroy();
      delete STriangle::shaderIF;
      STriangle::shaderIF = NULL;
      STriangle::shaderProgram = 0;
    }
}

void STriangle::defineModel()
{
  vec3* points = new vec3[VERTICES];
  vec3* normals = new vec3[VERTICES];

  vec3 Vertices[4] = {
    { -1.0f, -1.0f, 0.0f },
    { 0.0f, -1.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f }
  };

  vec3 Normals[4] = {
    { -1.0f, 0.5f, 1.0f },
    { 1.0f, 0.5f, 1.0f },
    { -1.0f, 0.5f, 1.0f },
    { 0.0f, 1.0f, 0.0f }
  };

  unsigned int Indices[] = { 0, 3, 1,
			      1, 3, 2,
			      2, 3, 0,
			      0, 1, 2 };

  for( int i = 0; i < VERTICES; ++i )
    {
      memcpy( points[i], Vertices[ Indices[ i ] ], sizeof( vec3 ) );
      normals[i][0] = 0;
      normals[i][1] = 0;
      normals[i][2] = 1;
    }

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 2, vbo );
  /* 1. coordinate data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, VERTICES * sizeof( vec3 ), points, GL_STATIC_DRAW );
  glVertexAttribPointer( STriangle::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( STriangle::pvaLoc_wcPosition );

  /* 2. normal data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, VERTICES * sizeof( vec3 ), normals, GL_STATIC_DRAW );
  glVertexAttribPointer( STriangle::pvaLoc_wcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( STriangle::pvaLoc_wcNormal );

  delete [] points;
  delete [] normals;
}

void STriangle::fetchGLSLVariableLocations()
{
  /* get the locations of the GLSL variables on the GPU */
  if( STriangle::shaderProgram > 0 ) /* if a shader program has been initialized... */
    {
      STriangle::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      STriangle::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
    }
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void STriangle::getWCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1; // xmin
	xyzLimits[1] = 1;  // xmax
	xyzLimits[2] = -1; // ymin
	xyzLimits[3] = 1; // ymax
	xyzLimits[4] = -1; // zmin
	xyzLimits[5] = 1;  // zmax
}

void STriangle::render()
{
  GLint pgm;
  short i;

  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  glUseProgram( shaderProgram );

  /*
  float scaleTrans[4];
  computeScaleTrans( scaleTrans );

put into the WCECMatrix...
  */

  glBindVertexArray( vao );
  
  for( i = 0; i < FACES /* faces of square */; ++i )
    {
      glDrawArrays( GL_TRIANGLES, VERTICESPERFACE*i, VERTICESPERFACE*(i+1) - 1 );
    }

  /* restore the previous program */
  glUseProgram( pgm );
}
