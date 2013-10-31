// GeneralMV.c++

#include "GeneralMV.h"
#include "ShaderIF.h"
#include "AffVector.h"
#include "AffPoint.h"
#include <cstring>
#include "Controller.h"

#include <iostream>
using namespace std;

/* static */ ShaderIF * GeneralMV::shaderIF = NULL;
/* static */ int GeneralMV::numInstances = 0;
/* static */ GLuint GeneralMV::shaderProgram = 0;

/* static */ GLint GeneralMV::ppuLoc_M4x4_wc_ec = -1;
/* static */ GLint GeneralMV::ppuLoc_M4x4_ec_lds = -1;
/* static */ GLint GeneralMV::ppuLoc_lightModel = -1;
/* static */ GLint GeneralMV::ppuLoc_kd = -1;
/* static */ GLint GeneralMV::pvaLoc_wcPosition = -1;
/* static */ GLint GeneralMV::pvaLoc_wcNormal = -1;

/* static */ vec3 GeneralMV::_eyeloc = { 46.4f, 22.6f, -8.0f };

/* static */ vec3 GeneralMV::_up = { 0.0f, 1.0f, 0.0f };
/* static */ vec3 GeneralMV::_centerloc = { 0.0f, 0.0f, 0.0f }; // for now just the origin
/* static */ vec3 GeneralMV::_lightloc = { 0.0f, 0.0f, -1.0f }; // for now directly from the eye location

/* static */ vec3 GeneralMV::_ecmin = { -110.1f, -22.0f, -115.8f };
/* static */ vec3 GeneralMV::_ecmax = {    9.4f,  29.5f,  110.4f };


GeneralMV::GeneralMV()
{
  if( GeneralMV::shaderProgram == 0 )
    {
      GeneralMV::shaderIF = new ShaderIF( "simple3d.vsh", "simple3d.fsh" );
      GeneralMV::shaderProgram = shaderIF->getShaderPgmID();

      fetchGLSLVariableLocations();
    }
} /* end GeneralMV::GeneralMV() */

GeneralMV::~GeneralMV()
{
  // since this is the base class
  // the destructor of the derived class
  // will be called first.
  // so it is the responsiblity of the
  // derived class to delete
  // its vao and vbo variables

  if( --GeneralMV::numInstances == 0 )
    {
      GeneralMV::shaderIF->destroy();
      delete GeneralMV::shaderIF;
      GeneralMV::shaderIF = NULL;
      GeneralMV::shaderProgram = 0;
    }
} /* end GeneralMV::~GeneralMV() */

void GeneralMV::printEyeLoc()
{
  cout << "Eye location: (" << _eyeloc[0] << "," << _eyeloc[1] << "," << _eyeloc[2] << ")" << endl;
}

void GeneralMV::printBox()
{
  cout << "Box:" << endl;
  cout << "x: (" << _ecmin[0] << "," << _ecmax[0] << ")" << endl;
  cout << "y: (" << _ecmin[1] << "," << _ecmax[1] << ")" << endl;
  cout << "z: (" << _ecmin[2] << "," << _ecmax[2] << ")" << endl;
}

void GeneralMV::handleCommand( unsigned char key, double ldsX, double ldsY )
{
#define MOVSPEED 0.1

  if( key == 'w' )
    {
      _eyeloc[2] += MOVSPEED;
    }
  else if( key == 's' )
    {
      _eyeloc[2] -= MOVSPEED;
    }
  else if( key == 'a' )
    {
      _eyeloc[0] -= MOVSPEED;
    }
  else if( key == 'd' )
    {
      _eyeloc[0] += MOVSPEED;
    }
  else if( key == 'r' )
    {
      _eyeloc[1] += MOVSPEED;
    }
  else if( key == 'f' )
    {
      _eyeloc[1] -= MOVSPEED;
    }
  else if( key == 'o' )
    {
      _eyeloc[0] = 0.0f;
      _eyeloc[1] = 0.0f;
      _eyeloc[2] = 5.0f;
      _ecmin[0] = -2.0f;
      _ecmin[1] = -2.0f;
      _ecmin[2] = -2.0f;
      _ecmax[0] = 2.0f;
      _ecmax[1] = 2.0f;
      _ecmax[2] = 2.0f;
    }
  else if( key == 'i' )
    {
      _ecmin[2] += MOVSPEED;
      _ecmax[2] += MOVSPEED;
    }
  else if( key == 'k' )
    {
      _ecmin[2] -= MOVSPEED;
      _ecmax[2] -= MOVSPEED;
    }
  else if( key == 'j' )
    {
      _ecmin[0] -= MOVSPEED;
      _ecmax[0] -= MOVSPEED;
    }
  else if( key == 'l' )
    {
      _ecmin[0] += MOVSPEED;
      _ecmax[0] += MOVSPEED;
    }
  else if( key == 'y' )
    {
      _ecmin[1] -= MOVSPEED;
      _ecmax[1] -= MOVSPEED;
    }
  else if( key == 'u' )
    {
      _ecmin[1] += MOVSPEED;
      _ecmax[1] += MOVSPEED;
    }

  else if( key == 'z' )
    {
      _ecmax[2] += MOVSPEED;
    }

  else if( key == 'n' )
    {
      _ecmin[2] -= MOVSPEED;
    }

  printEyeLoc();
  printBox();
} /* end GeneralMV::handleCommand() */

void GeneralMV::getMatrices( double limits[6] )
{
  /* I think there should be a model matrix created to move
     from model coordinates to world coordinates, but that
     doesn't seem to be facilitated here. So the models
     are just created in world coordinates */

  // Get the View matrix, Model matrix is I since already in WC
  // this matrix does move from wc -> ec
  wcToECMatrix(
	       _eyeloc[0], _eyeloc[1], _eyeloc[2],
	       _centerloc[0], _centerloc[1], _centerloc[2],
	       _up[0], _up[1], _up[2],
	       _model_view
	       );

  // Get the orthogonal projection matrix
  orthogonal(
	     _ecmin[0], _ecmax[0], _ecmin[1],
	     _ecmax[1], _ecmin[2], _ecmax[2],
	     _projection
	     );
  
  glUniformMatrix4fv( GeneralMV::ppuLoc_M4x4_wc_ec, 1, GL_FALSE, _model_view );
  glUniformMatrix4fv( GeneralMV::ppuLoc_M4x4_ec_lds, 1, GL_FALSE, _projection );
} /* end GenralMV::getMatrices() */

/**
 * The lighting model for this project
 * is a very simple diffusive light model.
 */
void GeneralMV::sendLightSource(
				vec4 kd    //!> The base color of the object
				)
{
  glUniform4fv( GeneralMV::ppuLoc_kd, 1, kd );
  glUniform3fv( GeneralMV::ppuLoc_lightModel, 1, _lightloc );
} /* end GeneralMV::sendLightSource() */

void GeneralMV::fetchGLSLVariableLocations()
{
  if( GeneralMV::shaderProgram > 0 )
    {
      GeneralMV::ppuLoc_M4x4_wc_ec = ppUniformLocation( shaderProgram, "M4x4_wc_ec" );
      GeneralMV::ppuLoc_M4x4_ec_lds = ppUniformLocation( shaderProgram, "M4x4_ec_lds" );
      GeneralMV::ppuLoc_kd = ppUniformLocation( shaderProgram, "kd" );
      GeneralMV::ppuLoc_lightModel = ppUniformLocation( shaderProgram, "lightModel" );
      GeneralMV::pvaLoc_wcPosition = pvAttribLocation( shaderProgram, "wcPosition" );
      GeneralMV::pvaLoc_wcNormal = pvAttribLocation( shaderProgram, "wcNormal" );
    }

} /* end GeneralMV::fetchGLSLVariableLocations() */

void GeneralMV::tri( const vec3& veca,
		     const vec3& vecb,
		     const vec3& vecc,
		     int& Index           //!> The index in _points and _normals at which to start
		     )
{
  cryph::AffVector a( veca );
  cryph::AffVector b( vecb );
  cryph::AffVector c( vecc );

  /* based on the example function quad() from Interactive Computer Graphics by E. Angel, p. 627 */
  cryph::AffVector normal = cryph::AffVector::cross( b - a, c - b );
  normal.normalize();

  normal.vComponents( _normals[Index] ); a.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); b.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); c.vComponents( _points[Index] ); Index++;
} /* end GeneralMV::tri() */

void GeneralMV::quad( 
		     int a,
		     int b,
		     int c,
		     int d,
		     int& Index          //!> The index in _points and _normals at which to start
		      )
{
  cryph::AffVector u = cryph::AffVector( _vertices[b] ) - cryph::AffVector( _vertices[a] );
  cryph::AffVector v = cryph::AffVector( _vertices[c] ) - cryph::AffVector( _vertices[b] );

  /* copied (with some modifications) quad() from Interactive Computer Graphics by E. Angel, p. 627 */
  //cryph::AffVector normal = cryph::AffVector::cross( b - a, c - b );
  cryph::AffVector normal = cryph::AffVector::cross( u, v );
  normal.normalize();
  /*
  normal.vComponents( _normals[Index] ); a.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); b.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); c.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); a.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); c.vComponents( _points[Index] ); Index++;
  normal.vComponents( _normals[Index] ); d.vComponents( _points[Index] ); Index++;
  */

  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[b], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[c], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[a], sizeof( vec3 ) ); Index++;
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[c], sizeof( vec3 ) ); Index++;  
  normal.vComponents( _normals[Index] ); memcpy( _points[Index], _vertices[d], sizeof( vec3 ) ); Index++;
} /* end GeneralMV::quad() */
