// Square.c++

#include "Square.h"
#include "ShaderIF.h"
#include "AffVector.h"
#include "MatrixIF.h"
#include <cstring>
#include <cstdio>
#include "Controller.h"

#define NumVertices 36

ShaderIF* Square::shaderIF = NULL;
int Square::numInstances = 0;
GLuint Square::shaderProgram = 0;

GLint Square::ppuLoc_M4x4_wc_ec = -1;
GLint Square::ppuLoc_M4x4_ec_lds = -1;
GLint Square::ppuLoc_kd = -1;
GLint Square::pvaLoc_wcPosition = -1;
GLint Square::pvaLoc_wcNormal = -1;

vec3 Square::_eye = { 0.0f, -3.0f, 0.0f /* -20.0f, -5.0f, -40.0f */ }; // in WC
vec3 Square::_center = { -3.0f, 0.0f, -3.0f }; // in WC
vec3 Square::_up = { 0.0f, 0.0f, -1.0f };
vec3 Square::_eyemin = { -1.0f, -1.0f, -1.0f /* 0.0f, 3.0f, 20.0f */ }; // in EC
vec3 Square::_eyemax = { 1.0f, 0.5f, 9.0f /* 40.0f, 6.0f, 40.0f */ }; // in EC


Square::Square( bool color, vec3 corner, float width, float thickness )
  :
  Index( 0 )
{
  if( Square::shaderProgram == 0 )
    {
      Square::shaderIF = new ShaderIF( "simple3d.vsh", "simple3d.fsh" );
      Square::shaderProgram = shaderIF->getShaderPgmID();
      
      fetchGLSLVariableLocations();
    }
  /*
 vec3 tmpvertices[8] = {
   { -0.5f, -0.5f, 0.5f  },
   { -0.5f, 0.5f, 0.5f },
   { 0.5f, 0.5f, 0.5f },
   { 0.5f, -0.5f, 0.5f },
   { -0.5f, -0.5f, -0.5f },
    { -0.5f, 0.5f, -0.5f },
    { 0.5f, 0.5f, -0.5f },
    { 0.5f, -0.5f, -0.5f }
 };
  */

  vec3 tmpvertices[8] = {
    { corner[0], corner[1], corner[2] },
    { corner[0], corner[1] + thickness, corner[2] },
    { corner[0] + width, corner[1] + thickness, corner[2] },
    { corner[0] + width, corner[1], corner[2] },
    { corner[0], corner[1], corner[2] - width },
    { corner[0], corner[1] + thickness, corner[2] - width },
    { corner[0] + width, corner[1] + thickness, corner[2] - width },
    { corner[0] + width, corner[1], corner[2] - width }
  };

  limits[0] = corner[0]; limits[1] = corner[0] + width;
  limits[2] = corner[1]; limits[3] = corner[1] + thickness;
  limits[4] = corner[2] - thickness; limits[5] = corner[2];

  for( int i = 0; i < 6; ++i )
    {
      if( i & 1 )
	limits[i] += 0.5;
      else
	limits[i] -= 0.5;
    }

  /*
  for ( short i = 0; i < 3; ++i )
    {
      Square::_eyemin[i] = static_cast<float>( limits[i*2] ) - Square::_eye[i] - 2;
      Square::_eyemax[i] = static_cast<float>( limits[i*2+1] ) - Square::_eye[i] + 2;
    }
  */

 for( short i = 0; i < 8; ++i )
   memcpy( vertices[i], tmpvertices[i], sizeof( vec3 ) );

 if( color )
   {
     _kd[0] = 1.0f;
     _kd[1] = 0.0f;
     _kd[2] = 0.0f;
     _kd[3] = 1.0f;
   }
 else
   {
     _kd[0] = 0.0941f;
     _kd[1] = 0.0941f;
     _kd[2] = 0.0941f;
     _kd[3] = 1.0f;
   }

  defineModel();
}

Square::~Square()
{
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );

  if( --numInstances == 0 )
    {
      Square::shaderIF->destroy();
      delete Square::shaderIF;
      Square::shaderIF = NULL;
      Square::shaderProgram = 0;
    }
}

void Square::fetchGLSLVariableLocations()
{
  if( Square::shaderProgram > 0 )
    {
      // TODO
      // get other GLSL vars
      Square::ppuLoc_M4x4_wc_ec = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
      Square::ppuLoc_M4x4_ec_lds = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );
      Square::ppuLoc_kd = ppUniformLocation( shaderProgram, "kd" );
      Square::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      Square::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
      
    }
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void Square::getWCBoundingBox(double* xyzLimits) const
{
  memcpy( xyzLimits, limits, 6 * sizeof( double ) );
}

void Square::render()
{
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  
  glUseProgram( shaderProgram );

  getMatrices();

  // send the transformation matrices and the kd value to the GPU
  glUniformMatrix4fv( Square::ppuLoc_M4x4_wc_ec, 1, GL_TRUE, _model_view );
  glUniformMatrix4fv( Square::ppuLoc_M4x4_ec_lds, 1, GL_TRUE, _projection );
  glUniform4fv( Square::ppuLoc_kd, 1, _kd );

  glBindVertexArray( vao );

  glDrawArrays( GL_TRIANGLES, 0, NumVertices );


  glUseProgram( pgm );
}

void Square::defineModel()
{
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );

  char buff[256];

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( points ), points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  sprintf(buff, "defineModel(): position" );
  Controller * a = Controller::getCurrentController();
  a->checkForErrors( std::cout, buff );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  sprintf(buff, "glBindBuffer" );
  a->checkForErrors( std::cout, buff );
  glBufferData( GL_ARRAY_BUFFER, sizeof( normals ), normals, GL_STATIC_DRAW );
  sprintf(buff, "glBufferData" );
  a->checkForErrors( std::cout, buff );
  glEnableVertexAttribArray( Square::pvaLoc_wcNormal );
  sprintf(buff, "glEnableVertexAttribArray" );
  a->checkForErrors( std::cout, buff );
  glVertexAttribPointer( Square::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
  sprintf(buff, "glVertexAttribPointer");
  a->checkForErrors( std::cout, buff );

} /* end Square::defineModel() */

void Square::quad( int a, int b, int c, int d )
{
  // copied (with some modification) from Interactive Computer Graphics by E. Angel
  cryph::AffVector u = cryph::AffVector( vertices[b] ) - cryph::AffVector( vertices[a] );
  cryph::AffVector v = cryph::AffVector( vertices[c] ) - cryph::AffVector( vertices[b] );

  cryph::AffVector normal = cryph::AffVector::cross( u, v );
  normal.normalize();

  /* normals[Index] - normal;           points[Index] = vertices[..];                         Index++ */
  normal.vComponents( normals[Index] ); memcpy( points[Index], vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( normals[Index] ); memcpy( points[Index], vertices[b], sizeof( vec3 ) ); Index++;
  normal.vComponents( normals[Index] ); memcpy( points[Index], vertices[c], sizeof( vec3 ) ); Index++;
  normal.vComponents( normals[Index] ); memcpy( points[Index], vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( normals[Index] ); memcpy( points[Index], vertices[c], sizeof( vec3 ) ); Index++;  
  normal.vComponents( normals[Index] ); memcpy( points[Index], vertices[d], sizeof( vec3 ) ); Index++;
} /* end Square::quad() */

void Square::getMatrices()
{
  // Get the Model_View matrix
  wcToECMatrix( 
	       _eye[0], _eye[1], _eye[2],
	       _center[0], _center[1], _center[3],
	       _up[0], _up[1], _up[2],
	       _model_view );

  // Get the Orthogonal projection matrix
  orthogonal(
	     _eyemin[0], _eyemax[0], // xmin/max in EC
	     _eyemin[1], _eyemax[1], // ymin/max
	     _eyemin[2], _eyemax[2], // zmin/max
	     _projection
	     );
} /* end Square::getMatrices() */
