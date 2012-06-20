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

struct Color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

struct PolygonOffset
{
	float factor;
	float units;
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
	virtual void draw( Primitive mode, vector<vec3d> vData ) {}
	virtual void draw( Primitive mode, int size, vector<double> vData ) {}
	virtual void draw( Primitive mode, int size, vector<double> vData, PolygonOffset offset ) {}
	virtual void draw( Primitive mode, int size, float* matrix, vector<double> vData ) {}
	virtual void draw( Primitive mode, Color color, vector<vec3d> vData ) {}
	virtual void draw( Primitive mode, vector<Color> colors, vector<vec3d> vData ) {}

protected:
	virtual unsigned int getGLPrimitiveMode( Primitive mode ) { return 0; }
};
#endif