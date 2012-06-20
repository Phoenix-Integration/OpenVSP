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

/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
GL11Renderer::GL11Renderer() : IRenderer()
{
}
GL11Renderer::~GL11Renderer()
{
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
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, vector<vec3d> data )
{
	glVertexPointer( 3, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( getGLPrimitiveMode( mode ), 0, data.size() * 3 );
	glDisableClientState( GL_VERTEX_ARRAY );
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, int size, vector<double> data )
{
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( getGLPrimitiveMode( mode ), 0, data.size() / size );
	glDisableClientState( GL_VERTEX_ARRAY );
}

/******************************************************
*
* Draw Geometry with glPolygonOffset.
*
*******************************************************
*
* mode - rendering mode. Example: R_POINT, R_LINES.
* size - number of the coordinate per vertex.  Example; 2, 3, 4.
* data - vector of doubles.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, int size, vector<double> data, PolygonOffset offset )
{
	glPolygonOffset( offset.factor, offset.units );

#ifndef __APPLE__
	glEnable( GL_POLYGON_OFFSET_EXT );
#endif
	
	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( getGLPrimitiveMode( mode ), 0, data.size() / size );
	glDisableClientState( GL_VERTEX_ARRAY );

#ifndef __APPLE__
	glDisable( GL_POLYGON_OFFSET_EXT );
#endif
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, int size, float* matrix, vector<double> data )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)matrix );

	glVertexPointer( size, GL_DOUBLE, 0, &data[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( getGLPrimitiveMode( mode ), 0, data.size() / 3 );
	glDisableClientState( GL_VERTEX_ARRAY );

	glPopMatrix();
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, Color color, vector<vec3d> data )
{
	glVertexPointer( 3, GL_DOUBLE, 0, &data[0] );
	
	glColor4ub( color.red, color.green, color.blue, color.alpha );

	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawArrays( getGLPrimitiveMode( mode ), 0, data.size() * 3 );
	glDisableClientState( GL_VERTEX_ARRAY );
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void GL11Renderer::draw( Primitive mode, vector<Color> colors, vector<vec3d> data )
{
	glVertexPointer( 3, GL_DOUBLE, 0, &data[0] );
	glVertexPointer( 4, GL_UNSIGNED_BYTE, 0, &colors[0] );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glDrawArrays( getGLPrimitiveMode( mode ), 0, data.size() * 3 );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

/******************************************************
*
* Get GL primitive enum.
*
*******************************************************/
unsigned int GL11Renderer::getGLPrimitiveMode( Primitive mode )
{
	unsigned int openglMode;
	switch ( mode )
	{
		case R_POINT:
			openglMode = GL_POINT;
			break;

		case R_LINE:
			openglMode = GL_LINE;
			break;

		case R_LINE_STRIP:
			openglMode = GL_LINE_STRIP;
			break;

		case R_LINE_LOOP:
			openglMode = GL_LINE_LOOP;
			break;

		case R_TRIANGLES:
			openglMode = GL_TRIANGLES;
			break;

		case R_TRIANGLE_STRIP:
			openglMode = GL_TRIANGLE_STRIP;
			break;

		case R_TRIANGLE_FAN:
			openglMode = GL_TRIANGLE_FAN;
			break;

		case R_QUADS:
			openglMode = GL_QUADS;
			break;

		case R_QUAD_STRIP:
			openglMode = GL_QUAD_STRIP;
			break;

		case R_POLYGON:
			openglMode = GL_POLYGON;
			break;

		default:
			openglMode = GL_POINT;
			break;
	}
	return openglMode;
}