#ifndef _TMESH_RENDERER_H
#define _TMESH_RENDERER_H

#include "vec3d.h"

/******************************************************
*
* Triangle Mesh Renderer.
*
*******************************************************/
class tMeshRenderer
{
public:
	tMeshRenderer();
	virtual ~tMeshRenderer();

public:
	virtual void draw();
	virtual void draw_wire();
	virtual void draw_shaded();

public:
	virtual void setValue( vector< vec3d > value, int ID );

protected:
	vector< vec3d > vertVec;	// vertex info
};

#endif