//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// materialMgr.h: interface for the materialMgr class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_TEXTUREMGR_H__635B9760_A025_11D7_AC31_0003473A025A__INCLUDED_)
#define AFX_TEXTUREMGR_H__635B9760_A025_11D7_AC31_0003473A025A__INCLUDED_

#include "IRenderer.h"
#include "GLTextureLoader.h"

class AppliedTex
{
public:
	AppliedTex();
	virtual ~AppliedTex()		{}

	virtual void extractName( const char* filename );

   Stringc nameStr;
	Stringc texStr;

	bool allSurfFlag;		// Apply To All Surfs 
	int surfID;				// Which Surface For This Texture
	unsigned int texID;

	double u;				// Pos UW
	double w;		
	double scaleu;
	double scalew;

	bool wrapUFlag;
	bool wrapWFlag;

	bool repeatFlag;
	double bright;
	double alpha;

	bool flipUFlag;
	bool flipWFlag;
	bool reflFlipUFlag;
	bool reflFlipWFlag;

};

class TextureMgr
{
public:

	TextureMgr();
	virtual ~TextureMgr();

	int loadTex( const char* name);

protected:
	ITextureLoader * textureLoaderPtr;
};


class SingleTexMgr
{
public:
	VSPDLL SingleTexMgr();
	TextureMgr* texMgr;
};

static SingleTexMgr stm;
#define texMgrPtr (stm.texMgr)

#endif