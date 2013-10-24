// TEMPLATE_Subclass.h

#ifndef TEMPLATE_SUBCLASS_H
#define TEMPLATE_SUBCLASS_H

#include "ModelView.h"

class TEMPLATE_Subclass : public ModelView
{
public:
	TEMPLATE_Subclass();
	virtual ~TEMPLATE_Subclass();

	// xyzLimits: {wcXmin, wcXmax, wcYmin, wcYmax, wcZmin, wcZmax}
	void getWCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	static void fetchGLSLVariableLocations();
};

#endif
