#ifndef _RENDER_MGR_H
#define _RENDER_MGR_H

#include "IRenderer.h"
#include "GL11Renderer.h"

class renderMgr
{
public:
	renderMgr();
	virtual ~renderMgr();

public:
	virtual void setColor3ub( unsigned char red, unsigned char green, unsigned char blue );
	virtual void setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
	virtual void setColor3d( double red, double green, double blue );
	virtual void setColor4d( double red, double green, double blue, double alpha );

	virtual void getColor4d( double color[4] );

public:
	virtual void setLight( int index, float* ambient, float* diffuse, float* specular, float* position );
	virtual void enableLight( int index );
	virtual void disableLight( int index );

public:
	virtual void setLineWidth( float width );
	virtual void enableLineSmooth( bool enableFlag );

	virtual void setPointSize( float size );
	virtual void enablePointSmooth( bool enableFlag );

	virtual void setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine );

	virtual void transform( double * tMatrix );

public:
	virtual void createGLWindow();

	virtual void setWindowSize( int x, int y, int width, int height );

	virtual void setClearColor( unsigned char r, unsigned char g, unsigned char b );

	virtual void setProjection( double left, double right, double top, double bottom, double near, double far );

	virtual void setBackgroundImage( float x, float y, float width, float height, float scaleW, float scaleH, unsigned char * imageData );
	virtual void removeBackgroundImage();

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

public:
	virtual void bindAttrib( RenderProperties rp );
	virtual void releaseAttrib();

	virtual void bindMatrix();
	virtual void releaseMatrix();

	virtual void loadIdentity();

public:
	RenderProperties rp_draw3D;
	RenderProperties rp_draw2D;
	RenderProperties rp_hidden;
	RenderProperties rp_shaded;
	RenderProperties rp_texture;

protected:
	IRenderer* renderPtr;
};

class SingleRenderer
{
public:
	SingleRenderer();
	virtual ~SingleRenderer();

	renderMgr * renMgr;
};

static SingleRenderer renderPtr;
#define renderer (renderPtr.renMgr)

#endif