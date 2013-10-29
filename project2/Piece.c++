#include "Piece.h"
#include <cstring>

GLint Piece::pvaLoc_wcPosition = -1;
GLint Piece::pvaLoc_wcNormal = -1;

Piece::Piece( vec3 center, float width, float height ) :
  Index( 0 )
{
  if( Piece::pvaLoc_wcPosition == -1 )
    fetchGLSLVariableLocations();

  float lefttop = ( center[0] - width ) * 0.4;
  float righttop = ( center[0] + width ) * 0.4;
  float backtop = ( center[2] + width ) * 0.4;
  float fronttop = ( center[2] + width ) * 0.4;
  float btm = center[1] + height;
  float top = center[1] + ( height * 1.2 );

  vec3 tmpvertices[NumDiffVertices] = {
    { center[0] - width, center[1], center[2] + width },
    { center[0] + width, center[1], center[2] + width },
    { center[0] + width, center[1], center[2] - width },
    { center[0] - width, center[1], center[2] - width },
    { center[0], center[1] + height, center[2] },
    // top of the piece
    { lefttop, btm, fronttop },
    { lefttop, top, fronttop },
    { righttop, top, fronttop },
    { righttop, btm, fronttop },
    { lefttop, btm, backtop },
    { lefttop, top, backtop },
    { righttop, top, backtop },
    { righttop, btm, backtop }
  };

  for( short i = 0; i < 8; ++i )
    memcpy( vertices[i], tmpvertices[i], sizeof( vec3 ) );

  limits[0] = center[0] - width; limits[1] = center[0] + width;
  limits[2] = center[1]; limits[3] = top;
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

  glDrawArrays( GL_TRIANGLES, 0, 18 /* PieceNumVertices */ /* + NumSphereVertices */ );

  glUseProgram( pgm );
}

void Piece::defineModel() {
  quad( 0, 3, 1, 2 );
  triangle( 2, 3, 4 );
  triangle( 3, 0, 4 );
  triangle( 1, 2, 4 );
  triangle( 0, 1, 4 );

  quad( 6, 5, 8, 7 );
  quad( 7, 8, 12, 11 );
  quad( 8, 5, 9, 12 );
  quad( 11, 10, 6, 7 );
  quad( 9, 10, 11, 12 );
  quad( 10, 9, 5, 6 );

  // maybe for a later project
  //tetrahedron( NumTimesToSubdivide );

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

void Piece::divide_triangle( const cryph::AffVector& a, const cryph::AffVector& b, const cryph::AffVector& c, int count )
{
  if( count > 0 )
    {
      cryph::AffVector v1 = a + b;
      v1.normalize();

      cryph::AffVector v2 = a + c;
      v2.normalize();

      cryph::AffVector v3 = b + c;
      v3.normalize();

      divide_triangle( a, v1, v2, count - 1 );
      divide_triangle( c, v2, v3, count - 1 );
      divide_triangle( b, v3, v1, count - 1 );
      divide_triangle( v1, v3, v2, count - 1 );
    }
  else
    {
      pointTriangle( a, b, c );
    }
}

void Piece::pointTriangle( const cryph::AffVector& a, const cryph::AffVector& b, const cryph::AffVector& c )
{
  cryph::AffVector normal = cryph::AffVector::cross( b - a, c - b );
  normal.normalize();

  normal.vComponents(normals[Index]); a.vComponents(points[Index]); Index++;
  normal.vComponents(normals[Index]); b.vComponents(points[Index]); Index++;
  normal.vComponents(normals[Index]); c.vComponents(points[Index]); Index++;
}

void Piece::tetrahedron( int count ) {
  cryph::AffVector v[4] = {
    cryph::AffVector( 0.0, 0.0, 1.0 ),
    cryph::AffVector( 0.0, 0.942809, -0.333333 ),
    cryph::AffVector( -0.816497, -0.471405, -0.333333 ),
    cryph::AffVector( 0.816497, -0.471405, -0.333333 )
  };

  divide_triangle( v[0], v[1], v[2], count );
  divide_triangle( v[3], v[2], v[1], count );
  divide_triangle( v[0], v[3], v[1], count );
  divide_triangle( v[0], v[2], v[3], count );

}

void Piece::fetchGLSLVariableLocations() {
  if( Piece::shaderProgram > 0 )
    {
      Piece::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      Piece::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
    }
}
