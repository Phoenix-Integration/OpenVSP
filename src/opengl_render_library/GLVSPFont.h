#ifndef _OPENGL_FONT_LOADER_H
#define _OPENGL_FONT_LOADER_H

#include "IRenderer.h"

#include "glfont2.h"
using namespace glfont;

class GLVSPFont : public IVSPFont
{
public:
	GLVSPFont();
	virtual ~GLVSPFont();

public:
	virtual int loadFont( const char * file );

public:
	virtual void draw( Stringc str, float scale, float x0, float y0, float xoffset, float yoffset );

protected:
	GLFont * font;

private:
	vector< pair< Stringc, GLFont * > > fontVec;
};
#endif