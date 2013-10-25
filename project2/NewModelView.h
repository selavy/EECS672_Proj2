#ifndef NEWMODELVIEW_H
#define NEWMODELVIEW_H

typedef float mat4[16];
typedef float vec3[3];
typedef float vec4[4];

#include "ModelView.h"
#include "ShaderIF.h"

class NewModelView : public ModelView
{
 public:
  NewModelView();
  virtual ~NewModelView();

  void getWCBoundingBox( double* xyzLimitsF ) const;
  void sendMatrices();
  void sendKD();

 protected:
  static ShaderIF* shaderIF;
  static int numInstances;
  static GLuint shaderProgram;
  static GLint ppuLoc_M4x4_wc_ec;
  static GLint ppuLoc_M4x4_ec_lds;
  static GLint ppuLoc_kd;

  static void fetchGLSLMatrixLocations();

  static vec3 _eye;
  static vec3 _center;
  static vec3 _up;
  static vec3 _eyemin;
  static vec3 _eyemax;

  double limits[6];

  vec4 _kd;
  mat4 _model_view;
  mat4 _projection;

 private:
    void getMatrices();
};

#endif
