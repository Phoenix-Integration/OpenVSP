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

struct LightingMode
{
	bool enabled;

	LightingMode():enabled(false) {}
};

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

struct DepthMaskMode
{
	bool enabled;

	DepthMaskMode():enabled(false) {}
};

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

struct LineSmoothMode
{
	bool enabled;

	LineSmoothMode():enabled(false) {}
};

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
		LineSmoothMode lineSmoothMode;

		RenderMode():alphaTestMode(), blendMode(), depthMaskMode(), depthTestMode(), lightingMode(), polygonOffsetMode(), 
			texture2DMode(), cullFaceMode(), lineSmoothMode() {}
	}	mode;

	RenderProperties():mode() {}
};

class IRenderer
{
public:
	IRenderer(){};
	virtual ~IRenderer(){};

public:
	virtual void setColor3ub( unsigned char red, unsigned char green, unsigned char blue ) {}
	virtual void setColor3d( double red, double green, double blue ) {}
	virtual void setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha ) {}
	virtual void setColor4d( double red, double green, double blue, double alpha ) {}

	virtual void getColor4d( double color[4] ) {}

public:
	virtual void setLineWidth( float width ) {}
	virtual void setPointSize( float size ) {}
	virtual void transform( double * tMatrix ) {}
	virtual void setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine ) {}

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
};

class ITextureLoader
{
public:
	ITextureLoader() {}
	virtual ~ITextureLoader() {}

public:
	virtual int loadTex( const char* name ) { return 0; }
};

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