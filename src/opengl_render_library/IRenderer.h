#ifndef _OPENGL_RENDER_INTERFACE_H
#define _OPENGL_RENDER_INTERFACE_H

#include "util.h"

enum Primitive
{
	R_POINT,
	R_LINE,
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
	struct Params
	{
		BlendMask sourcefactor;
		BlendMask destinationfactor;
		
		Params():sourcefactor( R_ZERO ), destinationfactor( R_ZERO ) {}
	}	params ;

	BlendMode():enabled(false), params() {}
};

struct PolygonOffsetMode
{
	bool enabled;	
	struct Params
	{
		float factor;
		float units;

		Params():factor(0), units(0) {}
	}	params;

	PolygonOffsetMode():enabled(false), params() {}
}; 

struct LightingMode
{
	bool enabled;

	LightingMode():enabled(false) {}
};

struct RenderProperties
{
	struct RenderMode
	{
		BlendMode blend;
		PolygonOffsetMode polygonOffset;
		LightingMode lighting;

		RenderMode():blend(), polygonOffset(), lighting() {}
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

public:
	virtual void draw( Primitive mode, int size, vector<double> data ) {}

	virtual void draw( Primitive mode, Color color, vector<vec3d> data ) {}
	virtual void draw( Primitive mode, vector<Color> colors, vector<vec3d> data ) {}

	virtual void draw( Primitive mode, int size, float* matrix, vector<double> data ) {}

	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data ) {}
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals ) {}

protected:
	virtual unsigned int getGLPrimitiveMode( Primitive mode ) { return 0; }
	virtual unsigned int getGLBlendMode( BlendMask mask ) { return 0; }
};
#endif