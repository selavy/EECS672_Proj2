// SimpleBox.c++

#include "SimpleBox.h"
#include <cstring>

#define __DEBUG__

#ifdef __DEBUG__
#include <iostream>
#endif

#define FACES 6
#define VERTICESPERFACE 4
#define VERTICES 8
#define NUMPOINTS ( VERTICESPERFACE * VERTICES )

#define ALPHAVALUE 1

/* static */ ShaderIF * SimpleBox::shaderIF = NULL;
/* static */ int        SimpleBox::numInstances = 0;
/* static */ GLuint     SimpleBox::shaderProgram = 0;
/* static */ GLint      SimpleBox::pvaLoc_wcPosition = -1;
/* static */ GLint      SimpleBox::pvaLoc_wcNormal = -1;
/* static */ GLint      SimpleBox::ppuLoc_M4x4_ec_lds = -1;
/* static */ GLint      SimpleBox::ppuLoc_M4x4_wc_ec = -1;
/* static */ GLint      SimpleBox::ppuLoc_kd = -1;

SimpleBox::SimpleBox()
{
#ifdef __DEBUG__
  std::cout << "Creating instance #" << numInstances + 1 << " of SimpleBox..." << std::endl;
#endif

  /* if the first time an instance of this class is generated, then create a common shader program */
  if( SimpleBox::shaderProgram == 0 )
    {
      SimpleBox::shaderIF = new ShaderIF( "SimpleBox.vsh", "SimpleBox.fsh" );
      SimpleBox::shaderProgram = shaderIF->getShaderPgmID();
      fetchGLSLVariableLocations();
    }

  defineModel();
  SimpleBox::numInstances++;
}

SimpleBox::~SimpleBox()
{
  glDeleteBuffers( 2, vbo );
  glDeleteVertexArrays( 1, &vao );

  if( (--SimpleBox::numInstances) == 0 )
    {
      SimpleBox::shaderIF->destroy();
      delete SimpleBox::shaderIF;
      SimpleBox::shaderIF = NULL;
      SimpleBox::shaderProgram = 0;
    }
}

void SimpleBox::defineModel()
{
  vec3* points = new vec3[NUMPOINTS];
  vec3* normals = new vec3[NUMPOINTS];

  /*
  float _x = 0.0f;
  float _y = 0.0f;
  float _thickness = 0.3f;
  float _width = 0.5f;
  */

  vec3 Vertices[VERTICES] = {
  /*
   * define the square
   */
    /*
    // front left
    { _x , 0, _y },
    { _x, -_thickness, _y },

    // front right
    { _x + _width, 0, _y  },
    { _x + _width, -_thickness, _y },

    // back right
    { _x + _width, 0, _y + _width },
    { _x + _width, -_thickness, _y + _width },

    // back left
    { _x, 0, _y + _width },
    { _x, -_thickness, _y + _width }
    */

    { 0.0f, 0.5f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.5f, 0.5f, 0.0f },
    { 0.5f, 0.0f, 0.0f },
    { 1.0f, 1.0f, -1.0f },
    { 0.5f, 0.5f, -1.0f },
    { 0.0f, 1.0f, -1.0f },
    { 0.0f, 0.5f, -1.0f }
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
    { 0.0f, 1.0f, 0.0f }, // top
    { 0.0f, -1.0f, 0.0f }, // bottom
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
  glVertexAttribPointer( SimpleBox::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( SimpleBox::pvaLoc_wcPosition );

  /* 2. normal data */
  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, NUMPOINTS * sizeof( vec3 ), normals, GL_STATIC_DRAW );
  glVertexAttribPointer( SimpleBox::pvaLoc_wcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( SimpleBox::pvaLoc_wcNormal );

  delete [] points;
  delete [] normals;
}

void SimpleBox::fetchGLSLVariableLocations()
{
  /* get the locations of the GLSL variables on the GPU */
  if( SimpleBox::shaderProgram > 0 ) /* if a shader program has been initialized... */
    {
      SimpleBox::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      SimpleBox::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
      SimpleBox::ppuLoc_M4x4_ec_lds = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );
      SimpleBox::ppuLoc_M4x4_wc_ec = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
    }
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void SimpleBox::getWCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1; // xmin
	xyzLimits[1] = 1;  // xmax
	xyzLimits[2] = -1; // ymin
	xyzLimits[3] = 1; // ymax
	xyzLimits[4] = -1; // zmin
	xyzLimits[5] = 1;  // zmax
}

void SimpleBox::render()
{
  GLint pgm;
  short i;

  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  glUseProgram( shaderProgram );

  mat4 WCtoECMatrix;
  getWCtoECMatrix( WCtoECMatrix );

  mat4 ECtoLDSMatrix;
  getECtoLDSMatrix( ECtoLDSMatrix );

  glUniformMatrix4fv( SimpleBox::ppuLoc_M4x4_wc_ec, 1, GL_TRUE, &WCtoECMatrix[0] );
  glUniformMatrix4fv( SimpleBox::ppuLoc_M4x4_ec_lds, 1, GL_TRUE, &ECtoLDSMatrix[0] );

  vec4 kd = { 1.0, 0.0, 0.0, 1.0 };

  glUniform1fv( SimpleBox::ppuLoc_kd, 4, &kd[0] );

  glBindVertexArray( vao );
  
  for( i = 0; i < 6 /* faces of square */; ++i )
    {
      glDrawArrays( GL_TRIANGLE_STRIP, 4*i, 4*(i+1) - 1 );
    }

  /* restore the previous program */
  glUseProgram( pgm );
}

void SimpleBox::getWCtoECMatrix( float * mat ) {
  mat4 aMatrix = { 1.0, 0.0, 0.0, 0.0,
	      0.0, 1.0, 0.0, 0.0,
	      0.0, 0.0, 1.0, 0.0,
	      0.0, 0.0, 0.0, 1.0 };

  for( short i = 0; i < 16; ++i )
    mat[i] = aMatrix[i];
}

void SimpleBox::getECtoLDSMatrix( float * mat ) {
  mat4 aMatrix = { 1.0, 0.0, 0.0, 0.0,
		   0.0, 1.0, 0.0, 0.0,
		   0.0, 0.0, -1.0, 0.0,
		   0.0, 0.0, 0.0, 1.0 };

  for( short i = 0; i < 16; ++i )
    mat[i] = aMatrix[i];
}
