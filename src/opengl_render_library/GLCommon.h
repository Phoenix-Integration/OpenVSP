#ifndef _OPENGL_RENDERER_COMMON_H
#define _OPENGL_RENDERER_COMMON_H

#include"IRenderer.h"

class GLCommon
{
public:
	GLCommon() {}
	virtual ~GLCommon() {}

public:
	virtual void getGLPrimitiveMode( Primitive mode, unsigned int &out );
	virtual void getGLBlendMode( BlendMask mask, unsigned int &out );
	virtual void getGLTexParamName( TexParamNameMask mask, unsigned int &out );
	virtual void getGLParameter( ParameterMask mask, unsigned int &out );
};
#endif