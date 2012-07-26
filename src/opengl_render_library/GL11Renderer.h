#ifndef _OPENGL_VERSION_1_1_RENDERER_H
#define _OPENGL_VERSION_1_1_RENDERER_H

#include "IRenderer.h"
#include "GLCommon.h"

/******************************************************
*
* GL11Renderer Header Class.
*
* Implementation base on OpenGL 1.1 standard.
* This class provides basic functionalities for Rendering.
*
*******************************************************/
class GL11Renderer : public IRenderer
{
public:
	GL11Renderer();
	virtual ~GL11Renderer();

/* Color */
public:
	virtual void setColor3ub( unsigned char red, unsigned char green, unsigned char blue );
	virtual void setColor3d( double red, double green, double blue );
	virtual void setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
	virtual void setColor4d( double red, double green, double blue, double alpha );

	virtual void getColor4d( double color[4] );

/* Light */
public:
	virtual void setLight( int index, float* ambient, float* diffuse, float* specular, float* position );
	virtual void enableLight( int index );
	virtual void disableLight( int index );

/* Utility */
public:
	virtual void setLineWidth( float width );
	virtual void enableLineSmooth( bool enableFlag );

	virtual void setPointSize( float size );
	virtual void enablePointSmooth( bool enableFlag );

	virtual void setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine );

	virtual void getModelViewMatrix( float * model_mat, float* refl_mat, float * view_mat_out, float * model_mat_out, float * refl_mat_out );

	virtual void transform( double * tMatrix );

/* Window / View Port */
public:
	virtual void createGLWindow();

	virtual void setWindowSize( int x, int y, int width, int height );

	virtual void setClearColor( unsigned char r, unsigned char g, unsigned char b );

	virtual void setProjection( double left, double right, double top, double bottom, double clipNear, double clipFar );

	virtual void setBackgroundImage( float x, float y, float width, float height, float scaleW, float scaleH, unsigned char * imageData );
	virtual void removeBackgroundImage();

/* Render Buffer */
public:
	virtual void clearBuffer();
	virtual void getBackBufferImage( unsigned char* data_out );

/* Draw Functions */
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

/* State Control */
public:
	virtual void bindAttrib( RenderProperties rp );
	virtual void releaseAttrib();

	virtual void bindMatrix();
	virtual void releaseMatrix();

	virtual void loadIdentity();

protected:
	GLCommon* commonUtil;
	int winX, winY, winW, winH;

	double orthoL, orthoR, orthoT, orthoB, orthoN, orthoF;

	double jpgImgX, jpgImgY, jpgImgW, jpgImgH, jpgImgScaleW, jpgImgScaleH;
	unsigned char * jpgImgData;

	unsigned char clearR;
	unsigned char clearG; 
	unsigned char clearB;
};
#endif