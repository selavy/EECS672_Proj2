// BoardMV.c++

#include "BoardMV.h"
#include "Controller.h"

#include "MatrixIF.h"

#define __DEBUG__

/* static */ ShaderIF * BoardMV::shaderIF = NULL;
/* static */ int        BoardMV::numInstances = 0;
/* static */ GLuint     BoardMV::shaderProgram = -1;
/* static */ GLint      BoardMV::pvaLoc_wcPosition = -1;
/* static */ GLint      BoardMV::pvaLoc_wcNormal = -1;
/* static */ GLint      BoardMV::ppuLoc_KD = -1;
/* static */ GLint      BoardMV::ppuLoc_WCECMatrix = -1;
/* static */ GLint      BoardMV::ppuLoc_ECLDSMatrix = -1;
/* static */ mat4       BoardMV::_orthogonal;
/* static */ mat4       BoardMV::_wcec;
/* static */ float      BoardMV::_eyexyz[3];

BoardMV::BoardMV()
{
  if( BoardMV::shaderProgram == 0 )
    {
      BoardMV::shaderIF = new ShaderIF( "Checkers.vsh", "Checkers.fsh" );
      BoardMV::shaderProgram = shaderIF->getShaderPgmID();
      fetchGLSLVariableLocations();
    }

  if( BoardMV::numInstances == 0 )
    {
      /* Just hard coding these for now */
      BoardMV::_eyexyz[0] = 10.0; // eye_x
      BoardMV::_eyexyz[1] = 5.0;  // eye_y
      BoardMV::_eyexyz[2] = 2.0;  // eye_z
    }

  BoardMV::numInstances++;
}

BoardMV::~BoardMV()
{
  if( (--BoardMV::numInstances) == 0 )
    {
      BoardMV::shaderIF->destroy();
      delete BoardMV::shaderIF;
      BoardMV::shaderIF = NULL;
      BoardMV::shaderProgram = 0;
    }
}

void BoardMV::fetchGLSLVariableLocations()
{
  if( BoardMV::shaderProgram > 0 )
    {
      BoardMV::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      BoardMV::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
      BoardMV::ppuLoc_KD = ppUniformLocation( shaderProgram, "kd" );
      BoardMV::ppuLoc_WCECMatrix = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
      BoardMV::ppuLoc_ECLDSMatrix = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );
    }
}

/* Class Method */
/*
 * assumes that a vao has been bound before being called
 */
void BoardMV::sendMatrices()
{
  BoardMV::computeOrthogonalMatrix();
  BoardMV::computeWCECMatrix();

  glUniformMatrix4fv( BoardMV::ppuLoc_WCECMatrix, 1, GL_FALSE, &_wcec[0] );
  glUniformMatrix4fv( BoardMV::ppuLoc_ECLDSMatrix, 1, GL_FALSE, &_orthogonal[0] );
}

/* Class Method */
void BoardMV::computeOrthogonalMatrix()
{
  float tmpmat[16];

  double limits[6];
  double eyelimits[6];

  Controller* c = Controller::getCurrentController();
  c->getWCRegionOfInterest(limits);

  for( short i = 0; i < 6; i += 2 )
    {
      eyelimits[i] = limits[i] - BoardMV::_eyexyz[ i / 2 ];
      eyelimits[i+1] = limits[i+1] - BoardMV::_eyexyz[ i / 2 ];
    }

  orthogonal( eyelimits[0], eyelimits[1], eyelimits[2],
	      eyelimits[3], eyelimits[4], eyelimits[6],
	      tmpmat );

#ifdef __DEBUG__
  std::cout << "\n\nOrthogonal:\n";
#endif

  for( short i = 0; i < 16; ++i )
    {
#ifdef __DEBUG__
      if( i % 4 == 0 )
	std::cout << std::endl;
      std::cout << tmpmat[i] << " ";
#endif
      _orthogonal[i] = tmpmat[i];
    }

#ifdef __DEBUG__
  std::cout << std::endl;
#endif
}

/* Class Method */
void BoardMV::computeWCECMatrix()
{
  float tmpmat[16];

  double limits[6];
  double midpoint[3];

  Controller* c = Controller::getCurrentController();
  c->getWCRegionOfInterest(limits);

  midpoint[0] = ( limits[1] - limits[0] ) / 2.0;
  midpoint[1] = ( limits[3] - limits[2] ) / 2.0;
  midpoint[2] = ( limits[5] - limits[4] ) / 2.0;

#ifdef __DEBUG__
  std::cout << "Limits: " << std::endl;
  for( short i = 0; i < 6; i+=2 )
    std::cout << limits[i] << " " << limits[i+1] << std::endl;

  std::cout << "Center of Attention: " << std::endl;
  for( short i = 0; i < 3; ++i )
    {
      std::cout << midpoint[i] << ", ";
    }
  std::cout << std::endl;

  std::cout << "Eye: " << std::endl;
  for( short i = 0; i < 3; ++i )
    {
      std::cout << BoardMV::_eyexyz[i] << ", ";
    }
  std::cout << std::endl;
#endif

  wcToECMatrix( BoardMV::_eyexyz[0], BoardMV::_eyexyz[1], BoardMV::_eyexyz[2],
		midpoint[0], midpoint[1], midpoint[2],
		0.0, 0.1, 0.0,
		tmpmat );
#ifdef __DEBUG__
  std::cout << "\n\nWCEC:\n";  
#endif

  for( short i = 0; i < 16; ++i )
    {
#ifdef __DEBUG__
      if( i % 4 == 0 )
	std::cout << std::endl;
      std::cout << tmpmat[i] << " ";
#endif
      _wcec[i] = tmpmat[i];
    }

#ifdef __DEBUG__
  std::cout << std::endl;
#endif
}

