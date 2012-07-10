//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// materialMgr.cpp: implementation of the materialMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "textureMgr.h"

AppliedTex::AppliedTex()
{
	allSurfFlag = false;		
	surfID = 0;				

	texID = 0;

	u = 0.5;
	w = 0.5;
	scaleu = 1.0;
	scalew = 1.0;

	wrapUFlag = false;
	wrapWFlag = true;

	repeatFlag = false;
	bright = 0.6;
	alpha  = 1.0;

	flipUFlag = false;
	flipWFlag = false;
	reflFlipUFlag = true;
	reflFlipWFlag = false;

}

void AppliedTex::extractName( const char* filename )
{
	Stringc texname = filename;
	texname.change_from_to('/', ' ');
	texname.change_from_to('\\', ' ');
	int nword = texname.count_words();

	Stringc basename = texname.get_word( nword-1 );

	basename.remove_substring(".jpg", 1);
	basename.remove_substring(".jpeg", 1);
	basename.remove_substring(".tga", 1);
	basename.remove_substring(".png", 1);

	nameStr = basename;
}


TextureMgr::TextureMgr()
{
	textureLoaderPtr = new ITextureLoader();

	bool renderFlag = 1;
	if ( renderFlag )
	{
		delete textureLoaderPtr;
		textureLoaderPtr = new TextureLoader();
	}
}
TextureMgr::~TextureMgr()
{
	delete textureLoaderPtr;
}

int TextureMgr::loadTex( const char* name) 
{
	return textureLoaderPtr->loadTex( name );
}


SingleTexMgr::SingleTexMgr()
{
	static TextureMgr* ptr = 0; 

    if (!ptr) 
		ptr = new TextureMgr();

	texMgr = ptr;     
}
