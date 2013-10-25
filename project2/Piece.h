#ifndef PIECE_H
#define PIECE_H

#include "NewModelView.h"

#define PieceNumVertices 18
#define NumDiffVertices 5

class Piece : public NewModelView
{
 public:
  Piece( vec3 center, float width, float height );
  virtual ~Piece();

  void render();

 private:
  void defineModel();

  void triangle( int a, int b, int c );
  void quad( int a, int b, int c, int d );

  GLuint vao;
  GLuint vbo[2]; // 0: coordinate data, 1: normals

  static GLint pvaLoc_wcPosition;
  static GLint pvaLoc_wcNormal;
  static void fetchGLSLVariableLocations();

  int Index;

  vec3 points[PieceNumVertices];
  vec3 normals[PieceNumVertices];

  vec3 vertices[NumDiffVertices];

};

#endif
