#include "renderMgr.h"

/******************************************************
*
* renderMgr Class.
*
* This class manages the type of render library to 
* use.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
renderMgr::renderMgr()
{
	int renderFlag = 1;
	if ( renderFlag )
		renderCore = new GL11Renderer();
	else
		renderCore = new IRenderer();
}
renderMgr::~renderMgr()
{
	delete renderCore;
}