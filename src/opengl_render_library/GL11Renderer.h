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
	virtual void transform( double * tMatrix );
	virtual void loadIdentity();
	virtual void pushMatrix();
	virtual void popMatrix();
	virtual void setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine );

public:
	virtual void draw( Primitive mode, int size, vector<double> data );

	virtual void draw( Primitive mode, int csize, vector<double> colors, int size, vector<double> data );

	virtual void draw( Primitive mode, float* matrix, int size, vector<double> data );
	virtual void draw( Primitive mode, float* matrix, int csize, vector<double> colors, int size, vector<double> data );

	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data );
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals );
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, int tsize, vector<double> texcoords );
	virtual void draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords );

	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data );
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals );
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, int tsize, vector<double> texcoords );
	virtual void draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords );

public:
	virtual void drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, vector<double> data );
	virtual void drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, float* matrix, vector<double> data );

protected:
	virtual void bind( RenderProperties rp );
	virtual void unbind( RenderProperties rp );

protected:
	GLCommon* commonUtil;
};
#endif