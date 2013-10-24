#ifndef BOARDMV_H
#define BOARDMV_H

#include <GL/gl.h>

#include "ModelView.h"
#include "ShaderIF.h"

typedef float mat4[4*4];

class BoardMV : public ModelView
{
 public:
  BoardMV();
  virtual ~BoardMV();

  virtual void fetchGLSLVariableLocations();

  static void sendMatrices();

 protected:
  static ShaderIF * shaderIF;
  static int numInstances;
  static GLuint shaderProgram;
  static GLint pvaLoc_wcPosition;
  static GLint pvaLoc_wcNormal;
  static GLint ppuLoc_kd;
  static GLint ppuLoc_M4x4_wc_ec;
  static GLint ppuLoc_M4x4_ec_lds;

  static void computeOrthogonalMatrix();
  static void computeWCECMatrix();

  static mat4 _orthogonal;
  static mat4 _wcec;

  static float _eyexyz[3];
};

#endif