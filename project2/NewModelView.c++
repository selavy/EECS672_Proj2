#include "NewModelView.h"
#include "MatrixIF.h"
#include <cstring>

ShaderIF* NewModelView::shaderIF = NULL;
int NewModelView::numInstances = 0;
GLuint NewModelView::shaderProgram = 0;

GLint NewModelView::ppuLoc_M4x4_wc_ec = -1;
GLint NewModelView::ppuLoc_M4x4_ec_lds = -1;
GLint NewModelView::ppuLoc_kd = -1;
GLint NewModelView::ppuLoc_lightModel = -1;

vec3 NewModelView::_eye = { 0.0f, -2.0f, 2.0f };
vec3 NewModelView::_center = { 2.0f, 2.0f, 0.0f };
vec3 NewModelView::_up = { 0.0f, 1.0f, 0.0f };
vec3 NewModelView::_eyemin = { -1.0f, -0.5f, -1.0f };
vec3 NewModelView::_eyemax = { 1.0f, 0.5f, 1.0f };
vec3 NewModelView::_light = { 0.0f, 0.0f, 1.0f };

NewModelView::NewModelView() {
  if( NewModelView::shaderProgram == 0 )
    {
      NewModelView::shaderIF = new ShaderIF( "simple3d.vsh", "simple3d.fsh" );
      NewModelView::shaderProgram = shaderIF->getShaderPgmID();

      fetchGLSLMatrixLocations();
    }

  // default limits
  limits[0] = -1.0f;
  limits[1] = 1.0f;
  limits[2] = -1.0f;
  limits[3] = 1.0f;
  limits[4] = -1.0f;
  limits[5] = 1.0f;

  // default kd = blue
  _kd[0] = 0.0f;
  _kd[1] = 0.0f;
  _kd[2] = 1.0f;
  _kd[3] = 1.0f;
}

NewModelView::~NewModelView() {
  if( --numInstances == 0 )
    {
      NewModelView::shaderIF->destroy();
      delete NewModelView::shaderIF;
      NewModelView::shaderIF = NULL;
      NewModelView::shaderProgram = 0;
    }
}

void NewModelView::getWCBoundingBox( double* xyzLimitsF ) const
{
  memcpy( xyzLimitsF, limits, sizeof( double ) * 6 );
}

void NewModelView::getMatrices()
{
  // Get the Model_View matrix
  wcToECMatrix(
	       _eye[0], _eye[1], _eye[2],
	       _center[0], _center[1], _center[2],
	       _up[0], _up[1], _up[2],
	       _model_view );

  // Get the Orthogonal projection matrix
  orthogonal(
	     _eyemin[0], _eyemax[0],
	     _eyemin[1], _eyemax[1],
	     _eyemin[2], _eyemax[2],
	     _projection
	     );
}

void NewModelView::sendMatrices()
{
  if( ( NewModelView::ppuLoc_M4x4_wc_ec > -1 ) && ( NewModelView::ppuLoc_M4x4_ec_lds > -1 ) )
    {
      getMatrices();

      glUniformMatrix4fv( NewModelView::ppuLoc_M4x4_wc_ec, 1, GL_TRUE, _model_view );
      glUniformMatrix4fv( NewModelView::ppuLoc_M4x4_ec_lds, 1, GL_TRUE,_projection );
    }
}

void NewModelView::sendKD()
{
  if( NewModelView::ppuLoc_kd > -1 )
    {
      glUniform4fv( NewModelView::ppuLoc_kd, 1, _kd );
      glUniform3fv( NewModelView::ppuLoc_lightModel, 1, _light );
    }
}

void NewModelView::fetchGLSLMatrixLocations()
{
  if( NewModelView::shaderProgram > 0 )
    {
      NewModelView::ppuLoc_M4x4_wc_ec = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
      NewModelView::ppuLoc_M4x4_ec_lds = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );
      NewModelView::ppuLoc_kd = ppUniformLocation( shaderProgram, "kd" );
      NewModelView::ppuLoc_lightModel = ppUniformLocation( shaderProgram, "lightModel" );
    }
}
