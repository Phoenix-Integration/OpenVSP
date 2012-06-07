#ifndef _CFD_MESH_RENDERER_H
#define _CFD_MESH_RENDERER_H

#include "glext.h"
#include "tri_mesh_renderer.h"
#include "grid_density_renderer.h"

/******************************************************
*
* CFD Mesh Renderer.
*
*******************************************************/
class cfdMeshRenderer
{
public:
	cfdMeshRenderer();
	virtual ~cfdMeshRenderer();

/* Draw Functions */
public:
	virtual void draw();

/* Set Functions */
public:
	virtual void setValue( vector< tMeshRenderer* > value, int ID );
	virtual void reset();

protected:
	vector< tMeshRenderer* > surfaces;
};
#endif