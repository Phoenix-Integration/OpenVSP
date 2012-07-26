#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#endif

#include "GLCommon.h"
#include <assert.h>

/******************************************************
*
* GLCommon Class.
*
*******************************************************/
/******************************************************
*
* Get GL equivalent of Primitive enum.
*
*******************************************************/
void GLCommon::getGLPrimitiveMode( Primitive mask, GLuint &out )
{
	switch ( mask )
	{
		case R_POINTS:
			out = GL_POINTS;
			break;

		case R_LINES:
			out = GL_LINES;
			break;

		case R_LINE_STRIP:
			out = GL_LINE_STRIP;
			break;

		case R_LINE_LOOP:
			out = GL_LINE_LOOP;
			break;

		case R_TRIANGLES:
			out = GL_TRIANGLES;
			break;

		case R_TRIANGLE_STRIP:
			out = GL_TRIANGLE_STRIP;
			break;

		case R_TRIANGLE_FAN:
			out = GL_TRIANGLE_FAN;
			break;

		case R_QUADS:
			out = GL_QUADS;
			break;

		case R_QUAD_STRIP:
			out = GL_QUAD_STRIP;
			break;

		case R_POLYGON:
			out = GL_POLYGON;
			break;

		default:
			/* Shouldn't reach here */
			assert( false );
			out = GL_POINT;
			break;
	}
}

/******************************************************
*
* Get GL equivalent of Blend Type enum.
*
*******************************************************/
void GLCommon::getGLBlendMode( BlendMask mask, GLuint &out )
{
	switch ( mask )
	{
		case R_ZERO:
			out = GL_ZERO;
			break;

		case R_ONE:
			out = GL_ONE;
			break;
	
		case R_SRC_COLOR:
			out = GL_SRC_COLOR;
			break;

		case R_ONE_MINUS_SRC_COLOR:
			out = GL_ONE_MINUS_SRC_COLOR;
			break;

		case R_DST_COLOR:
			out = GL_DST_COLOR;
			break;

		case R_ONE_MINUS_DST_COLOR:
			out = GL_ONE_MINUS_DST_COLOR;
			break;

		case R_SRC_ALPHA:
			out = GL_SRC_ALPHA;
			break;

		case R_ONE_MINUS_SRC_ALPHA:
			out = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case R_DST_ALPHA:
			out = GL_DST_ALPHA;
			break;

		case R_ONE_MINUS_DST_ALPHA:
			out = GL_ONE_MINUS_DST_ALPHA;
			break;

		case R_CONSTANT_COLOR:
			out = GL_CONSTANT_COLOR;
			break;

		case R_ONE_MINUS_CONSTANT_COLOR:
			out = GL_ONE_MINUS_CONSTANT_COLOR;
			break;

		case R_CONSTANT_ALPHA:
			out = GL_CONSTANT_ALPHA;
			break;

		case R_ONE_MINUS_CONSTANT_ALPHA:
			out = GL_ONE_MINUS_CONSTANT_ALPHA;
			break;

		case R_SRC_ALPHA_SATURATE:
			out = GL_SRC_ALPHA_SATURATE;
			break;

		default:
			/* Shouldn't reach here, something must went wrong */
			assert( false );
			out = GL_ONE;
			break;
	}
}

/******************************************************
*
* Get GL equivalent of Texture Parameter enum.
*
*******************************************************/
void GLCommon::getGLTexParamName( TexParamNameMask mask, GLuint &out )
{
	switch( mask )
	{
		case R_TEXTURE_MIN_FILTER:
			out = GL_TEXTURE_MIN_FILTER;
			break;

		case R_TEXTURE_MAG_FILTER:
			out = GL_TEXTURE_MAG_FILTER;
			break;

		case R_TEXTURE_MIN_LOD:
			out = GL_TEXTURE_MIN_LOD;
			break;

		case R_TEXTURE_MAX_LOD:
			out = GL_TEXTURE_MAX_LOD;
			break;

		case R_TEXTURE_BASE_LEVEL:
			out = GL_TEXTURE_BASE_LEVEL;
			break;

		case R_TEXTURE_MAX_LEVEL:
			out = GL_TEXTURE_MAX_LEVEL;
			break;

		case R_TEXTURE_WRAP_S:
			out = GL_TEXTURE_WRAP_S;
			break;

		case R_TEXTURE_WRAP_T:
			out = GL_TEXTURE_WRAP_T;
			break;

		case R_TEXTURE_WRAP_R:
			out = GL_TEXTURE_WRAP_R;
			break;

		case R_TEXTURE_BORDER_COLOR:
			out = GL_TEXTURE_BASE_LEVEL;
			break;

		case R_TEXTURE_PRIORITY:
			out = GL_TEXTURE_PRIORITY;
			break;

		case R_TEXTURE_COMPARE_MODE:
			out = GL_TEXTURE_COMPARE_MODE;
			break;

		case R_TEXTURE_COMPARE_FUNC:
			out = GL_TEXTURE_COMPARE_FUNC;
			break;

		case R_DEPTH_TEXTURE_MODE:
			out = GL_DEPTH_TEXTURE_MODE;
			break;

		case R_GENERATE_MIPMAP:
			out = GL_GENERATE_MIPMAP;
			break;
	}
}

/******************************************************
*
* Get GL equivalent of Parameter enum.
*
*******************************************************/
void GLCommon::getGLParameter( ParameterMask mask, GLuint &out )
{
	switch ( mask )
	{
		case R_NEAREST:
			out = GL_NEAREST;
			break;

		case R_LINEAR:
			out = GL_LINEAR;
			break;

		case R_NEAREST_MIPMAP_NEAREST:
			out = GL_NEAREST_MIPMAP_NEAREST;
			break;

		case R_LINEAR_MIPMAP_NEAREST:
			out = GL_LINEAR_MIPMAP_NEAREST;
			break;

		case R_NEAREST_MIPMAP_LINEAR:
			out = GL_NEAREST_MIPMAP_LINEAR;
			break;

		case R_LINEAR_MIPMAP_LINEAR:
			out = GL_LINEAR_MIPMAP_LINEAR;
			break;

		case R_CLAMP:
			out = GL_CLAMP;
			break;

		case R_CLAMP_TO_BORDER:
			out = GL_CLAMP_TO_BORDER;
			break;

		case R_CLAMP_TO_EDGE:
			out = GL_CLAMP_TO_EDGE;
			break;

		case R_MIRRORED_REPEAT:
			out = GL_MIRRORED_REPEAT;
			break;

		case R_REPEAT:
			out = GL_REPEAT;
			break;

		case R_COMPARE_R_TO_TEXTURE:
			out = GL_COMPARE_R_TO_TEXTURE;
			break;

		case R_NONE:
			out = GL_NONE;
			break;

		case R_LUMINANCE:
			out = GL_LUMINANCE;
			break;

		case R_INTENSITY:
			out = GL_INTENSITY;
			break;

		case R_ALPHA:
			out = GL_ALPHA;
			break;

		case R_NEVER:
			out = GL_NEVER;
			break;

		case R_LESS:
			out = GL_LESS;
			break;

		case R_EQUAL:
			out = GL_EQUAL;
			break;

		case R_LEQUAL:
			out = GL_LEQUAL;
			break;

		case R_GREATER:
			out = GL_GREATER;
			break;

		case R_NOTEQUAL:
			out = GL_NOTEQUAL;
			break;

		case R_GEQUAL:
			out = GL_GEQUAL;
			break;

		case R_ALWAYS:
			out = GL_ALWAYS;
			break;

		case R_TRUE:
			out = GL_TRUE;
			break;

		case R_FALSE:
			out = GL_FALSE;
			break;

		case R_BACK:
			out = GL_BACK;
			break;

		case R_FRONT:
			out = GL_FRONT;
			break;

		case R_FRONT_AND_BACK:
			out = GL_FRONT_AND_BACK;
			break;

		default:
			assert( false );
			out = GL_NONE;
			break;
	}
}