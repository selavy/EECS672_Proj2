// TEMPLATE_Subclass.c++

#include "TEMPLATE_Subclass.h"
#include "ShaderIF.h"

TEMPLATE_Subclass::TEMPLATE_Subclass()
{
}

TEMPLATE_Subclass::~TEMPLATE_Subclass()
{
}

void TEMPLATE_Subclass::fetchGLSLVariableLocations()
{
}

// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
void TEMPLATE_Subclass::getWCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1000.0; // xmin  Give real values!
	xyzLimits[1] = 1000.0;  // xmax         |
	xyzLimits[2] = -1234.5; // ymin         |
	xyzLimits[3] = -1011.2; // ymax         |
	xyzLimits[4] = -1000.0; // zmin         |
	xyzLimits[5] = 1000.0;  // zmax        \_/
}

void TEMPLATE_Subclass::render()
{
}
