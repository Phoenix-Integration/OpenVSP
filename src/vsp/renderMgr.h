#ifndef _RENDER_MGR_H
#define _RENDER_MGR_H

#include "IRenderer.h"
#include "GL11Renderer.h"

/******************************************************
*
* renderMgr Class.
*
* This class manages the type of render library to 
* use.
*
*******************************************************/
class renderMgr
{
public:
	renderMgr();
	virtual ~renderMgr();

	IRenderer* renderCore;
};

static renderMgr renderMgrPtr;
#define renderer (renderMgrPtr.renderCore)

#endif