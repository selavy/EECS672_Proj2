#ifndef PIECE_H
#define PIECE_H

#include "NewModelView.h"
#include "AffVector.h"
#include "AffPoint.h"

#define PieceNumVertices 18 + 36
#define NumDiffVertices 5 + 8

/* // maybe for later project
#define NumTimesToSubdivide 5
#define NumTriangles 4096
#define NumSphereVertices ( 3 * NumTriangles )
*/

class Piece : public NewModelView
{
 public:
  Piece( vec3 center, float width, float height );
  virtual ~Piece();

  void render();

 private:
  void defineModel();

  void triangle( int a, int b, int c );
  void pointTriangle( const cryph::AffVector& a, const cryph::AffVector& b, const cryph::AffVector& c );
  void quad( int a, int b, int c, int d );
  void divide_triangle( const cryph::AffVector& a, const cryph::AffVector& b, const cryph::AffVector& c, int count );
  void tetrahedron( int count );

  GLuint vao;
  GLuint vbo[2]; // 0: coordinate data, 1: normals

  static GLint pvaLoc_wcPosition;
  static GLint pvaLoc_wcNormal;
  static void fetchGLSLVariableLocations();

  int Index;

  vec3 points[PieceNumVertices /* + NumSphereVertices */];
  vec3 normals[PieceNumVertices /* + NumSphereVertices */];

  vec3 vertices[NumDiffVertices];

};

#endif
