#ifndef _OPENGL_RENDER_INTERFACE_H
#define _OPENGL_RENDER_INTERFACE_H

#include "util.h"
#include "stringc.h"

#define FONT_BASE_SCALE (1.0 / (double)1024.0)

enum Primitive
{
	R_POINTS,
	R_LINES,
	R_LINE_STRIP,
	R_LINE_LOOP,
	R_TRIANGLES,
	R_TRIANGLE_STRIP,
	R_TRIANGLE_FAN,
	R_QUADS,
	R_QUAD_STRIP,
	R_POLYGON,
};

enum BlendMask
{
	R_ZERO,
	R_ONE,
	R_SRC_COLOR,
	R_ONE_MINUS_SRC_COLOR,
	R_DST_COLOR,
	R_ONE_MINUS_DST_COLOR,
	R_SRC_ALPHA,
	R_ONE_MINUS_SRC_ALPHA,
	R_DST_ALPHA,
	R_ONE_MINUS_DST_ALPHA,
	R_CONSTANT_COLOR,
	R_ONE_MINUS_CONSTANT_COLOR,
	R_CONSTANT_ALPHA,
	R_ONE_MINUS_CONSTANT_ALPHA,
	R_SRC_ALPHA_SATURATE,
};

enum TexParamNameMask
{
	R_TEXTURE_MIN_FILTER,
	R_TEXTURE_MAG_FILTER,
	R_TEXTURE_MIN_LOD,
	R_TEXTURE_MAX_LOD,
	R_TEXTURE_BASE_LEVEL,
	R_TEXTURE_MAX_LEVEL,
	R_TEXTURE_WRAP_S,
	R_TEXTURE_WRAP_T,
	R_TEXTURE_WRAP_R,
	R_TEXTURE_BORDER_COLOR,
	R_TEXTURE_PRIORITY,
	R_TEXTURE_COMPARE_MODE,
	R_TEXTURE_COMPARE_FUNC,
	R_DEPTH_TEXTURE_MODE,
	R_GENERATE_MIPMAP,
};

enum ParameterMask
{
	R_NEAREST,
	R_LINEAR,
	R_NEAREST_MIPMAP_NEAREST,
	R_LINEAR_MIPMAP_NEAREST,
	R_NEAREST_MIPMAP_LINEAR,
	R_LINEAR_MIPMAP_LINEAR,
	R_CLAMP,
	R_CLAMP_TO_BORDER,
	R_CLAMP_TO_EDGE,
	R_MIRRORED_REPEAT,
	R_REPEAT,
	R_COMPARE_R_TO_TEXTURE,
	R_NONE,
	R_LUMINANCE,
	R_INTENSITY,
	R_ALPHA,
	R_NEVER,
	R_LESS,
	R_EQUAL,
	R_LEQUAL,
	R_GREATER,
	R_NOTEQUAL,
	R_GEQUAL,
	R_ALWAYS,
	R_TRUE,
	R_FALSE,
	R_BACK,
	R_FRONT,
	R_FRONT_AND_BACK,
};

struct Color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

/******************************************************
*
* Blend state.
*
*******************************************************/
struct BlendMode
{
	bool enabled;
	struct BlendFunc
	{
		BlendMask sfactor;
		BlendMask dfactor;
		
		BlendFunc():sfactor( R_ZERO ), dfactor( R_ZERO ) {}
	}	blendfunc ;

	BlendMode():enabled(false), blendfunc() {}
};

/******************************************************
*
* PolygonOffsetMode.
*
*******************************************************/
struct PolygonOffsetMode
{
	bool enabled;	
	struct PolygonOffset
	{
		float factor;
		float units;

		PolygonOffset():factor(0), units(0) {}
	}	polygonOffset;

	PolygonOffsetMode():enabled(false), polygonOffset() {}
}; 

/******************************************************
*
* Lighting state.
*
*******************************************************/
struct LightingMode
{
	bool enabled;

	LightingMode():enabled(false) {}
};

/******************************************************
*
* Texture2D state.
*
*******************************************************/
struct Texture2DMode
{
	bool enabled;
	struct TexParameteri
	{
		vector<TexParamNameMask> pname;
		vector<ParameterMask> param;

		TexParameteri():pname(), param() {}
	}	texParameteri;
	struct BindTexture
	{
		unsigned int texture;

		BindTexture():texture(0) {}
	}	bindTexture;

	Texture2DMode():enabled(false), texParameteri(), bindTexture() {}
};

/******************************************************
*
* AlphaTest state.
*
*******************************************************/
struct AlphaTestMode
{
	bool enabled;
	struct AlphaFunc
	{
		ParameterMask func;
		float ref;

		AlphaFunc():func(R_ALWAYS), ref(0) {}
	}	alphafunc;

	AlphaTestMode():enabled(false), alphafunc() {}
};

/******************************************************
*
* DepthMask state.
*
*******************************************************/
struct DepthMaskMode
{
	bool enabled;

	DepthMaskMode():enabled(false) {}
};

/******************************************************
*
* DepthTest state.
*
*******************************************************/
struct DepthTestMode
{
	bool enabled;
	struct DepthFunc
	{
		ParameterMask func;

		DepthFunc():func(R_LESS) {}
	}	depthfunc;

	DepthTestMode():enabled(false), depthfunc() {}
};

/******************************************************
*
* Cull Face state.
*
*******************************************************/
struct CullFaceMode
{
	bool enabled;
	struct CullFace
	{
		ParameterMask mode;

		CullFace():mode(R_BACK) {}
	}	cullface;

	CullFaceMode():enabled(false), cullface() {}
};

/******************************************************
*
* Normalize.
*
*******************************************************/
struct NormalizeMode
{
	bool enabled;

	NormalizeMode():enabled(false) {}
};

/******************************************************
*
* Struct to keep track the render state.
*
*******************************************************/
struct RenderProperties
{
	struct RenderMode
	{
		AlphaTestMode alphaTestMode;
		BlendMode blendMode;
		DepthMaskMode depthMaskMode;
		DepthTestMode depthTestMode;
		LightingMode lightingMode;
		PolygonOffsetMode polygonOffsetMode;
		Texture2DMode texture2DMode;
		CullFaceMode cullFaceMode;
		NormalizeMode normalizeMode;

		RenderMode():alphaTestMode(), blendMode(), depthMaskMode(), depthTestMode(), lightingMode(), polygonOffsetMode(), 
			texture2DMode(), cullFaceMode(), normalizeMode() {}
	}	mode;

	RenderProperties():mode() {}
};

/******************************************************
*
* Interface for Graphic Render class.
*
*******************************************************/
class IRenderer
{
public:
	IRenderer();
	virtual ~IRenderer(){}

/* Color */
public:
	virtual void setColor3ub( unsigned char red, unsigned char green, unsigned char blue ) {}
	virtual void setColor3d( double red, double green, double blue ) {}
	virtual void setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha ) {}
	virtual void setColor4d( double red, double green, double blue, double alpha ) {}

	virtual void getColor4d( double color[4] ) {}

/* Light */
	virtual void setLight( int index, float* ambient, float* diffuse, float* specular, float* position ) {}
	virtual void enableLight( int index ) {}
	virtual void disableLight( int index ) {}

/* Utility */
public:
	virtual void setLineWidth( float width ) {}
	virtual void enableLineSmooth( bool enableFlag ) {}

	virtual void setPointSize( float size ) {}
	virtual void enablePointSmooth( bool enableFlag ){}

	virtual void setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine ) {}

	virtual void getModelViewMatrix( float * model_mat, float * refl_mat, float * view_mat_out, float * model_mat_out, float * refl_mat_out ) {}

	virtual void transform( double * tMatrix ) {}

/* Window / View Port */
public:
	virtual void createGLWindow() {}

	virtual void setWindowSize( int x, int y, int width, int height ) {}

	virtual void setClearColor( unsigned char r, unsigned char g, unsigned char b ) {}

	virtual void setProjection( double left, double right, double top, double bottom, double clipNear, double clipFar ) {}

	virtual void setBackgroundImage( float x, float y, float width, float height, float scaleW, float scaleH, unsigned char * imageData ) {}
	virtual void removeBackgroundImage() {}

/* Render Buffer */
public:
	virtual void clearBuffer() {}
	virtual void getBackBufferImage( unsigned char* data_out ) {}

/* Draw Functions */
public:
	virtual void draw( Primitive mode, int size, vector<double> data ) {}

	virtual void draw( Primitive mode, int csize, vector<double> colors, int size, vector<double> data ) {}

	virtual void draw( Primitive mode, float* matrix, int size, vector<double> data ) {}
	virtual void draw( Primitive mode, float* matrix, int csize, vector<double> colors, int size, vector<double> data ) {}

	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data ) {}
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals ) {}
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, int tsize, vector<double> texcoords ) {}
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords ) {}

	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data ) {}
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals ) {}
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, int tsize, vector<double> texcoords ) {}
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords ) {}

public:
	virtual void drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, vector<double> data ) {}
	virtual void drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, float* matrix, vector<double> data ) {}

/* State Control */
public:
	virtual void bindAttrib( RenderProperties rp ) {}
	virtual void releaseAttrib() {}

	virtual void bindMatrix() {}
	virtual void releaseMatrix() {}

	virtual void loadIdentity() {}

/* Render Style */
public:
	RenderProperties rp_draw3D;
	RenderProperties rp_draw2D;
	RenderProperties rp_hidden;
	RenderProperties rp_shaded;
	RenderProperties rp_texture;
};

/******************************************************
*
* Interface for Texture Loader.
*
*******************************************************/
class ITextureLoader
{
public:
	ITextureLoader() {}
	virtual ~ITextureLoader() {}

public:
	virtual int loadTex( const char* name ) { return 0; }
};

/******************************************************
*
* Interface for Font.
*
*******************************************************/
class IVSPFont
{
public:
	IVSPFont() {}
	virtual ~IVSPFont() {}

public:
	virtual int loadFont( const char* name ) { return 0; }

public:
	virtual void draw( Stringc str, float scale, float x0, float y0, float xoffset, float yoffset ) {}
	virtual void draw( Stringc str, float scale, float * trans_mat, float x0, float y0, float xoffset, float yoffset ) {}
};
#endif