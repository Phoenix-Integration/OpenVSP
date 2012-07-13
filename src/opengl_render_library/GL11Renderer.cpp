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
* Set Line Width.
*
*******************************************************/
void GL11Renderer::setLineWidth( float width )
{
	glLineWidth( width );
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
* Matrix Transformation.
*
*******************************************************/
void GL11Renderer::transform( double * tMatrix )
{
	glMultMatrixd( (GLdouble*)tMatrix );
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
	bind( rp );

	/* Draw */
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( pMode, 0, data.size() / size );
	glDisableClientState( GL_VERTEX_ARRAY );

	/* Restore Settings */
	unbind( rp );
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
	bind( rp );

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
	unbind( rp );
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
	bind( rp );

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
	unbind( rp );
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
	bind( rp );

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
	unbind( rp );
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
void GL11Renderer::bind( RenderProperties rp )
{
	/* PolygonOffset */
	if ( rp.mode.polygonOffsetMode.enabled )
	{
		glPolygonOffset( rp.mode.polygonOffsetMode.polygonOffset.factor, rp.mode.polygonOffsetMode.polygonOffset.units );

#ifndef __APPLE__
		glEnable( GL_POLYGON_OFFSET_EXT );
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

	/* Alpha test */
	if ( rp.mode.alphaTestMode.enabled )
	{
		unsigned int alphaTestfunc = 0;
		commonUtil->getGLParameter( rp.mode.alphaTestMode.alphafunc.func, alphaTestfunc );

		glAlphaFunc( alphaTestfunc, rp.mode.alphaTestMode.alphafunc.ref );
		glEnable( GL_ALPHA_TEST );
	}

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

	/* Lighting */
	if ( rp.mode.lightingMode.enabled )
		glEnable( GL_LIGHTING );

	/* Cull face */
	if ( rp.mode.cullFaceMode.enabled )
	{
		unsigned int cullmode = 0;
		commonUtil->getGLParameter( rp.mode.cullFaceMode.cullface.mode, cullmode );

		glCullFace( cullmode );
		glEnable( GL_CULL_FACE );
	}

	/* Line Smooth */
	if ( rp.mode.lineSmoothMode.enabled )
	{
		glEnable( GL_LINE_SMOOTH );
	}
}

/******************************************************
*
* Restore the changes.
*
*******************************************************/
void GL11Renderer::unbind( RenderProperties rp )
{
	/* Restore Settings */

	if ( rp.mode.lineSmoothMode.enabled )
		glDisable( GL_LINE_SMOOTH );

	if ( rp.mode.cullFaceMode.enabled )
		glDisable( GL_CULL_FACE );

	if ( rp.mode.lightingMode.enabled )
		glDisable( GL_LIGHTING );

	if ( rp.mode.blendMode.enabled )
		glDisable( GL_BLEND );

	if ( rp.mode.alphaTestMode.enabled )
		glDisable( GL_ALPHA_TEST );

	if ( rp.mode.texture2DMode.enabled )
		glDisable( GL_TEXTURE_2D );
	
	// FIXME: DepthMask mode and DepthTest mode need better
	// clean up.
	glDepthMask( GL_TRUE );
		
	glDepthFunc( GL_LESS );
	glEnable( GL_DEPTH_TEST );

	if ( rp.mode.polygonOffsetMode.enabled )
	{
#ifndef __APPLE__
		glDisable( GL_POLYGON_OFFSET_EXT );
#endif
	}
}