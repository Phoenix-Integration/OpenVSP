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
	virtual void init();

public:
	virtual void setColor3ub( unsigned char red, unsigned char green, unsigned char blue );
	virtual void setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
	virtual void setColor3d( double red, double green, double blue );
	virtual void setColor4d( double red, double green, double blue, double alpha );

	virtual void getColor4d( double color[4] );

public:
	virtual void setLineWidth( float width );
	virtual void setPointSize( float size );
	virtual void transform( double * tMatrix );

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

public:
	virtual void drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, vector<double> data );

protected:
	IRenderer* renderPtr;
};

#endif