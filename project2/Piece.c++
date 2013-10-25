#include "Piece.h"
#include <cstring>
#include "AffVector.h"
#include "AffPoint.h"

GLint Piece::pvaLoc_wcPosition = -1;
GLint Piece::pvaLoc_wcNormal = -1;

Piece::Piece( vec3 center, float width, float height ) :
  Index( 0 )
{
  if( Piece::pvaLoc_wcPosition == -1 )
    fetchGLSLVariableLocations();

  vec3 tmpvertices[NumDiffVertices] = {
    { center[0] - width, center[1], center[2] + width },
    { center[0] + width, center[1], center[2] + width },
    { center[0] + width, center[1], center[2] - width },
    { center[0] - width, center[1], center[2] - width },
    { center[0], center[1] + height, center[2] }
  };

  for( short i = 0; i < 8; ++i )
    memcpy( vertices[i], tmpvertices[i], sizeof( vec3 ) );

  limits[0] = center[0] - width; limits[1] = center[0] + width;
  limits[2] = center[1]; limits[3] = center[1] + height;
  limits[4] = center[2] - width; limits[5] = center[2] + width;

  for( short i = 0; i < 6; ++i )
    {
      if( i & 1 )
	limits[i] += 0.5;
      else
	limits[i] -= 0.5;
    }

  _kd[0] = 0.2f;
  _kd[1] = 0.2f;
  _kd[2] = 0.2f;
  _kd[3] = 1.0f;

  defineModel();
}

Piece::~Piece() {
  glDeleteBuffers( 2, &vbo[0] );
  glDeleteVertexArrays( 1, &vao );
}

void Piece::render() {
  GLint pgm;
  glGetIntegerv( GL_CURRENT_PROGRAM, &pgm );

  glUseProgram( shaderProgram );

  sendMatrices();
  sendKD();

  glBindVertexArray( vao );

  glDrawArrays( GL_TRIANGLES, 0, PieceNumVertices );

  glUseProgram( pgm );
}

void Piece::defineModel() {
  quad( 0, 3, 1, 2 );
  triangle( 2, 3, 4 );
  triangle( 3, 0, 4 );
  triangle( 1, 2, 4 );
  triangle( 0, 1, 4 );


  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 2, &vbo[0] );
  glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( points ), points, GL_STATIC_DRAW );
  glVertexAttribPointer( Piece::pvaLoc_wcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( Piece::pvaLoc_wcPosition );

  glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof( normals ), normals, GL_STATIC_DRAW );
  glEnableVertexAttribArray( Piece::pvaLoc_wcNormal );
  glVertexAttribPointer( Piece::pvaLoc_wcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
} /* end Piece::defineModel() */

void Piece::triangle( int a, int b, int c )
{
  cryph::AffVector * u = new cryph::AffVector( cryph::AffPoint( vertices[b] ) - cryph::AffPoint( vertices[a] ) );
  cryph::AffVector * v = new cryph::AffVector( cryph::AffPoint( vertices[c] ) - cryph::AffPoint( vertices[b] ) );

  cryph::AffVector * normal = new cryph::AffVector( cryph::AffVector::cross( *u, *v ) );
  normal->normalize();

  if( Index >= PieceNumVertices )
    Index = 0;

  normal->vComponents( normals[Index] ); memcpy( points[Index], vertices[a], sizeof( vec3 ) ); Index++;
  normal->vComponents( normals[Index] ); memcpy( points[Index], vertices[b], sizeof( vec3 ) ); Index++;
  normal->vComponents( normals[Index] ); memcpy( points[Index], vertices[c], sizeof( vec3 ) ); Index++;

   delete u;
   delete v;
   delete normal;
}

void Piece::quad( int a, int b, int c, int d )
{
  // copied (with some modification) from Interactive Computer Graphics by E. Angel, p.627
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
} /* end Piece::quad() */

void Piece::fetchGLSLVariableLocations() {
  if( Piece::shaderProgram > 0 )
    {
      Piece::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      Piece::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
    }
}
