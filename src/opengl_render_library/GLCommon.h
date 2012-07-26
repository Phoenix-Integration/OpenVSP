#ifndef _OPENGL_RENDERER_COMMON_H
#define _OPENGL_RENDERER_COMMON_H

#ifdef _WIN32
#	include <windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#else
#	include <gl\GL.h>
#	include "glext.h"
#endif

#include"IRenderer.h"

/******************************************************
*
* GLCommon Class.
*
* GLCommon shares common functionalities among different
* versions of OpenGL.
*
*******************************************************/
class GLCommon
{
public:
	GLCommon() {}
	virtual ~GLCommon() {}

public:
	virtual void getGLPrimitiveMode( Primitive mode, GLuint &out );
	virtual void getGLBlendMode( BlendMask mask, GLuint &out );
	virtual void getGLTexParamName( TexParamNameMask mask, GLuint &out );
	virtual void getGLParameter( ParameterMask mask, GLuint &out );
};
#endif