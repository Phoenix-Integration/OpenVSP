//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

#include "fontMgr.h"

FontMgr::FontMgr()
{
	VSPFontPtr = new IVSPFont();
	//VSPFontPtr = new GLVSPFont();
}

FontMgr::~FontMgr() 
{
	delete VSPFontPtr;
}

int FontMgr::loadFont( const char * file )
{
	return VSPFontPtr->loadFont( file );
}

void FontMgr::draw( Stringc str, float scale, float x0, float y0, float xoffset, float yoffset )
{
	VSPFontPtr->draw( str, scale, x0, y0, xoffset, yoffset );
}
