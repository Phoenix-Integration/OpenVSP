#ifndef _OPENGL_VERSION_1_1_RENDERER_H
#define _OPENGL_VERSION_1_1_RENDERER_H

#include "IRenderer.h"
#include "GLCommon.h"

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
	virtual void draw( Primitive mode, int size, vector<double> data );

	virtual void draw( Primitive mode, int csize, vector<double> colors, int size, vector<double> data );

	virtual void draw( Primitive mode, float* matrix, int size, vector<double> data );
	virtual void draw( Primitive mode, float* matrix, int csize, vector<double> colors, int size, vector<double> data );

	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data );
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals );
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals, vector<double> texcoords );

	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data );
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals );
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals, vector<double> texcoords );

protected:
	virtual void apply( RenderProperties rp );
	virtual void retract( RenderProperties rp );

protected:
	GLCommon* commonUtil;
};
#endif