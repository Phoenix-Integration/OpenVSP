#ifndef _OPENGL_VERSION_1_1_RENDERER_H
#define _OPENGL_VERSION_1_1_RENDERER_H

#include "IRenderer.h"

class GL11Renderer : public IRenderer
{
public:
	GL11Renderer();
	virtual ~GL11Renderer();

public:
	virtual void setColor3ub( unsigned char red, unsigned char green, unsigned char blue );
	virtual void setColor3d( double red, double green, double blue );
	virtual void setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
	virtual void setColor4d( double red, double green, double blue, double alpha );

	virtual void getColor4d( double color[4] );

public:
	virtual void setLineWidth( float width );
	virtual void setPointSize( float size );

public:
	virtual void draw( Primitive mode, vector<vec3d> data );
	virtual void draw( Primitive mode, int size, vector<double> data );
	virtual void draw( Primitive mode, int size, vector<double> data, PolygonOffset offset );
	virtual void draw( Primitive mode, int size, float* matrix, vector<double> data );
	virtual void draw( Primitive mode, Color color, vector<vec3d> data );
	virtual void draw( Primitive mode, vector<Color> colors, vector<vec3d> data );

protected:
	virtual unsigned int getGLPrimitiveMode( Primitive mode );
};
#endif