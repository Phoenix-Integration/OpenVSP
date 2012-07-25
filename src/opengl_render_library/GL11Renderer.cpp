#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#endif

#ifdef _WIN32
#	include <windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#else
#	include <gl\GL.h>
#	include "glext.h"
#endif

#include "GL11Renderer.h"
#include <assert.h>

/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
GL11Renderer::GL11Renderer() : IRenderer()
{
	commonUtil = new GLCommon();

	winX = winY = 0;
	winW = winH = 1;

	orthoL = orthoT = 1.0;
	orthoR = orthoB = 0.0;
	orthoN = -1.0;
	orthoF = 1.0;

	jpgImgX = jpgImgY = 0.0;
	jpgImgW = jpgImgH = jpgImgScaleW = jpgImgScaleH = 1.0;

	jpgImgData = NULL;

	clearR = 255;
	clearG = 255;
	clearB = 255;
}
GL11Renderer::~GL11Renderer()
{
	delete commonUtil;
}

/******************************************************
*
* Set Color. ( 3 unsigned bytes )
*
*******************************************************/
void GL11Renderer::setColor3ub( unsigned char red, unsigned char green, unsigned char blue )
{
	glColor3ub( red, green, blue );
}

/******************************************************
*
* Set Color. ( 3 doubles )
*
*******************************************************/
void GL11Renderer::setColor3d( double red, double green, double blue )
{
	glColor3d( red, green, blue );
}

/******************************************************
*
* Set Color. ( 4 unsigned bytes )
*
*******************************************************/
void GL11Renderer::setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
	glColor4ub( red, green, blue, alpha );
}

/******************************************************
*
* Set Color. ( 4 doubles )
*
*******************************************************/
void GL11Renderer::setColor4d( double red, double green, double blue, double alpha )
{
	glColor4d( red, green, blue, alpha );
}

/******************************************************
*
* Get Color. ( 4 doubles )
*
*******************************************************/
void GL11Renderer::getColor4d( double color[4] )
{
	glGetDoublev( GL_CURRENT_COLOR, color );
}

/******************************************************
*
* Set Light Source Parameters.
*
*******************************************************/
void GL11Renderer::setLight( int index, float* ambient, float* diffuse, float* specular, float* position )
{
	int max;
	glGetIntegerv( GL_MAX_LIGHTS, &max );
	assert( max - 1 > index );
	if ( index > max - 1 )
		return;

	GLenum light = GL_LIGHT0;
	light += index;

	glLightfv( light, GL_AMBIENT, (GLfloat*)ambient );
	glLightfv( light, GL_DIFFUSE, (GLfloat*)diffuse );
	glLightfv( light, GL_SPECULAR, (GLfloat*)specular );
	glLightfv( light, GL_POSITION, (GLfloat*)position );
}

/******************************************************
*
* Enable Light Source at Index.
*
*******************************************************/
void GL11Renderer::enableLight( int index )
{
	int max;
	glGetIntegerv( GL_MAX_LIGHTS, &max );
	assert( max - 1 > index );
	if ( index > max - 1 )
		return;

	GLenum light = GL_LIGHT0;
	light += index;

	glEnable( light );
}

/******************************************************
*
* Disable Light Source at Index.
*
*******************************************************/
void GL11Renderer::disableLight( int index )
{
	int max;
	glGetIntegerv( GL_MAX_LIGHTS, &max );
	assert( max - 1 > index );
	if ( index > max - 1 )
		return;

	GLenum light = GL_LIGHT0;
	light += index;

	glDisable( light );
}

/******************************************************
*
* Set Line Width.
*
*******************************************************/
void GL11Renderer::setLineWidth( float width )
{
	glLineWidth( width );
}

/******************************************************
*
* Activate or Deactivate Line Smooth.
*
*******************************************************/
void GL11Renderer::enableLineSmooth( bool enableFlag )
{
	if ( enableFlag )
		glEnable( GL_LINE_SMOOTH );
	else
		glDisable( GL_LINE_SMOOTH );
}

/******************************************************
*
* Set Point Size.
*
*******************************************************/
void GL11Renderer::setPointSize( float size )
{
	glPointSize( size );
}

/******************************************************
*
* Activate or Deactivate Point Smooth.
*
*******************************************************/
void GL11Renderer::enablePointSmooth( bool enableFlag )
{
	if ( enableFlag )
		glEnable( GL_POINT_SMOOTH );
	else
		glDisable( GL_POINT_SMOOTH );
}

/******************************************************
*
* Matrix Transformation.
*
*******************************************************/
void GL11Renderer::transform( double * tMatrix )
{
	glMultMatrixd( (GLdouble*)tMatrix );
}

/******************************************************
*
* Replace Current Matrix with Identity Matrix.
*
*******************************************************/
void GL11Renderer::loadIdentity()
{
	glLoadIdentity();
}

/******************************************************
*
* Store Matrix.
*
*******************************************************/
void GL11Renderer::bindMatrix()
{
	glPushMatrix();
}

/******************************************************
*
* Release Matrix.
*
*******************************************************/
void GL11Renderer::releaseMatrix()
{
	glPopMatrix();
}

/******************************************************
*
* Set Material.
*
*******************************************************/
void GL11Renderer::setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine )
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   amb );	
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diff );	
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  spec );	
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,  emiss );	
	glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS, shine );
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, int size, vector<double> data )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_VERTEX_ARRAY );
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, int csize, vector<double> colors, int size, vector<double> data )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );
	glColorPointer( csize, GL_DOUBLE, 0, &colors[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, float* matrix, int size, vector<double> data )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_VERTEX_ARRAY );

	glPopMatrix();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, float* matrix, int csize, vector<double> colors, int size, vector<double> data )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );
	glColorPointer( csize, GL_DOUBLE, 0, &colors[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glPopMatrix();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, int size, vector<double> data )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	/* Apply Properties */
	bindAttrib( rp );

	/* Draw */
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_VERTEX_ARRAY );

	/* Restore Settings */
	releaseAttrib();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	/* Apply Properties */
	bindAttrib( rp );

	/* Set Rendering Data */
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );
	glNormalPointer( GL_DOUBLE, 0, &normals[0] );

	/* Draw */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	/* Restore Settings */
	releaseAttrib();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, int tsize, vector<double> texcoords )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	/* Apply Properties */
	bindAttrib( rp );

	/* Set Rendering Data */
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );
	glTexCoordPointer( tsize, GL_DOUBLE, 0, &texcoords[0] );

	/* Draw */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	/* Restore Settings */
	releaseAttrib();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords )
{
	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	/* Apply Properties */
	bindAttrib( rp );

	/* Set Rendering Data */
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );
	glNormalPointer( GL_DOUBLE, 0, &normals[0] );
	glTexCoordPointer( tsize, GL_DOUBLE, 0, &texcoords[0] );

	/* Draw */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	/* Restore Settings */
	releaseAttrib();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	draw( mode, rp, size, data );

	glPopMatrix();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	draw( mode, rp, size, data, normals );

	glPopMatrix();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, int tsize, vector<double> texcoords )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	draw( mode, rp, size, data, tsize, texcoords );

	glPopMatrix();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	draw( mode, rp, size, data, normals, tsize, texcoords );

	glPopMatrix();
}

/******************************************************
*
* Draw Line Stipple ( data format - 3 doubles ).
*
* //FIXME: This function uses deprecated OpenGL API.
*
*******************************************************/
void GL11Renderer::drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, vector<double> data )
{
	double ver[3];

	glLineStipple( factor, pattern );
	glEnable( GL_LINE_STIPPLE );

	unsigned int pMode = 0;
	commonUtil->getGLPrimitiveMode( mode, pMode );

	glBegin( pMode );
	for( int i = 0; i < data.size(); i+=3 )
	{
		for ( int j = 0; j < 3; j++ )
		{
			ver[j] = data[i+j];
		}
		glVertex3dv( ver );
	}
	glEnd();

	glDisable( GL_LINE_STIPPLE );
}

/******************************************************
*
* Draw Line Stipple.
*
*******************************************************/
void GL11Renderer::drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, float* matrix, vector<double> data )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	drawLineStipple3d( factor, pattern, mode, data );

	glPopMatrix();
}

/******************************************************
*
* Apply Customized Rendering Properties.
*
*******************************************************/
void GL11Renderer::bindAttrib( RenderProperties rp )
{
	/* Store Attrib */
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	/* Normalize */
	if ( rp.mode.normalizeMode.enabled )
		glEnable( GL_NORMALIZE );
	else
		glDisable( GL_NORMALIZE );

	/* PolygonOffset */
	if ( rp.mode.polygonOffsetMode.enabled )
	{
		glPolygonOffset( rp.mode.polygonOffsetMode.polygonOffset.factor, rp.mode.polygonOffsetMode.polygonOffset.units );

#ifndef __APPLE__
		glEnable( GL_POLYGON_OFFSET_EXT );
#endif
	}
	else
	{
#ifndef __APPLE__
		glDisable( GL_POLYGON_OFFSET_EXT );
#endif
	}

	/* DepthMask */
	if ( rp.mode.depthMaskMode.enabled )
		glDepthMask( GL_TRUE );
	else
		glDepthMask( GL_FALSE );

	/* DepthTest */
	if ( rp.mode.depthTestMode.enabled )
	{
		unsigned int depthfunc = 0;
		commonUtil->getGLParameter( rp.mode.depthTestMode.depthfunc.func, depthfunc );

		glDepthFunc( depthfunc );
		glEnable( GL_DEPTH_TEST );
	}
	else
		glDisable( GL_DEPTH_TEST );

	/* Alpha test */
	if ( rp.mode.alphaTestMode.enabled )
	{
		unsigned int alphaTestfunc = 0;
		commonUtil->getGLParameter( rp.mode.alphaTestMode.alphafunc.func, alphaTestfunc );

		glAlphaFunc( alphaTestfunc, rp.mode.alphaTestMode.alphafunc.ref );
		glEnable( GL_ALPHA_TEST );
	}
	else
		glDisable( GL_ALPHA_TEST );

	/* Texture 2D */
	if ( rp.mode.texture2DMode.enabled )
	{
		assert( rp.mode.texture2DMode.texParameteri.pname.size() == rp.mode.texture2DMode.texParameteri.param.size() );

		glBindTexture( GL_TEXTURE_2D, rp.mode.texture2DMode.bindTexture.texture );

		unsigned int pname, param;
		pname = param = 0;
		for ( int i = 0; i < rp.mode.texture2DMode.texParameteri.pname.size(); i++ )
		{
			commonUtil->getGLTexParamName( rp.mode.texture2DMode.texParameteri.pname[i], pname );
			commonUtil->getGLParameter( rp.mode.texture2DMode.texParameteri.param[i], param );
			glTexParameteri( GL_TEXTURE_2D, pname, param );
		}
		glEnable( GL_TEXTURE_2D );
	}
	else
		glDisable( GL_TEXTURE_2D );

	/* Blend */
	if ( rp.mode.blendMode.enabled )
	{
		unsigned int sfactor, dfactor;
		sfactor = dfactor = 0;
		commonUtil->getGLBlendMode( rp.mode.blendMode.blendfunc.sfactor, sfactor );
		commonUtil->getGLBlendMode( rp.mode.blendMode.blendfunc.dfactor, dfactor );

		glBlendFunc( sfactor, dfactor );
		glEnable( GL_BLEND );
	}
	else
		glDisable( GL_BLEND );

	/* Lighting */
	if ( rp.mode.lightingMode.enabled )
		glEnable( GL_LIGHTING );
	else
		glDisable( GL_LIGHTING );

	/* Cull face */
	if ( rp.mode.cullFaceMode.enabled )
	{
		unsigned int cullmode = 0;
		commonUtil->getGLParameter( rp.mode.cullFaceMode.cullface.mode, cullmode );

		glCullFace( cullmode );
		glEnable( GL_CULL_FACE );
	}
	else
		glDisable( GL_CULL_FACE );
}

/******************************************************
*
* Restore the changes.
*
*******************************************************/
void GL11Renderer::releaseAttrib()
{
	/* Release Attrib */
	glPopAttrib();
}

/******************************************************
*
* Set GL Window Size.
*
*******************************************************/
void GL11Renderer::setWindowSize( int x, int y, int width, int height )
{
	winX = x;
	winY = y;
	winW = width;
	winH = height;
}

/******************************************************
*
* Set Projection.
*
*******************************************************/
void GL11Renderer::setProjection( double left, double right, double top, double bottom, double clipNear, double clipFar )
{
	orthoL = left;
	orthoR = right;
	orthoT = top;
	orthoB = bottom;
	orthoN = clipNear;
	orthoF = clipFar;
}

/******************************************************
*
* Set Background Color.
*
*******************************************************/
void GL11Renderer::setClearColor( unsigned char r, unsigned char g, unsigned char b )
{
	clearR = r;
	clearG = g;
	clearB = b;
}

/******************************************************
*
* Set Background Image.
*
*******************************************************/
void GL11Renderer::setBackgroundImage( float x, float y, float width, float height, float scaleW, float scaleH, unsigned char * imgData )
{
	jpgImgX = x;
	jpgImgY = y;
	jpgImgW = width;
	jpgImgH = height;
	jpgImgScaleW = scaleW;
	jpgImgScaleH = scaleH;
	jpgImgData = imgData;
}

/******************************************************
*
* Remove Background Image.
*
*******************************************************/
void GL11Renderer::removeBackgroundImage()
{
	jpgImgData = NULL;
}

/******************************************************
*
* Create GL Window.
*
* This function sets up a viewport.  If 
* setBackGroundImage() is called, this function also
* sets up the background image.
*
*******************************************************/
void GL11Renderer::createGLWindow()
{
	glEnable( GL_SCISSOR_TEST );

	/* Set Viewport */
	glViewport( winX, winY, winW, winH );
	glScissor( winX, winY, winW, winH );
	glClearColor( (float)clearR / 255.0f, (float)clearG / 255.0f, (float)clearB / 255.0f, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Apply Background Image */
	if ( jpgImgData )
	{
		glPushAttrib( GL_DEPTH_BUFFER_BIT );
		glDisable( GL_DEPTH_TEST );

		glRasterPos2d( jpgImgX, jpgImgY );
		glBitmap (0, 0, 0, 0, (float)(-jpgImgW/2), (float)(jpgImgH/2), NULL);
		glPixelZoom((float)jpgImgScaleW, (float)jpgImgScaleH);
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glDrawPixels( jpgImgW, jpgImgH, GL_RGB, GL_UNSIGNED_BYTE, jpgImgData );
		glPixelZoom(1.0,  1.0);

		glPopAttrib();
	}

	glDisable( GL_SCISSOR_TEST );

	/* Set Projection */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho( orthoL, orthoR, orthoT, orthoB, orthoN, orthoF );

	glMatrixMode( GL_MODELVIEW );
}