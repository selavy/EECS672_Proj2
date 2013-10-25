// Square.c++

#include "Square.h"
#include "ShaderIF.h"
#include "AffVector.h"
#include "MatrixIF.h"
#include <cstring>
#include <cstdio>
#include "Controller.h"

#define NumVertices 36

GLint Square::pvaLoc_wcPosition = -1;
GLint Square::pvaLoc_wcNormal = -1;

Square::Square( bool color, vec3 corner, float width, float thickness )
  :
  Index( 0 )
{
  if( Square::pvaLoc_wcPosition == -1 )
    fetchGLSLVariableLocations();

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
}

void Square::fetchGLSLVariableLocations()
{
  if( Square::shaderProgram > 0 )
    {
      Square::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      Square::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
      
    }
}

void Square::render()
{
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );
  
  glUseProgram( shaderProgram );

  sendMatrices();
  sendKD();

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

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( points ), points, GL_STATIC_DRAW );
  glVertexAttribPointer( Square::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Square::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( normals ), normals, GL_STATIC_DRAW );
  glEnableVertexAttribArray( Square::pvaLoc_wcNormal );
  glVertexAttribPointer( Square::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
} /* end Square::defineModel() */

void Square::quad( int a, int b, int c, int d )
{
  // copied (with some modification) from Interactive Computer Graphics by E. Angel, p. 627
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
