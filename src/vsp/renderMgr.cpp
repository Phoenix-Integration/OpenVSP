#include "renderMgr.h"

renderMgr::renderMgr()
{
	int renderFlag = 1;
	if ( renderFlag )
		renderPtr = new GL11Renderer();
	else
		renderPtr = new IRenderer();

	rp_draw3D.mode.blendMode.enabled = true;
	rp_draw3D.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_draw3D.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	rp_draw3D.mode.depthMaskMode.enabled = true;
	rp_draw3D.mode.depthTestMode.enabled = true;
	rp_draw3D.mode.depthTestMode.depthfunc.func = R_LESS;
	rp_draw3D.mode.normalizeMode.enabled = true;

	rp_draw2D.mode.blendMode.enabled = true;
	rp_draw2D.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_draw2D.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	rp_draw2D.mode.depthMaskMode.enabled = true;

	/* Customized Render Properties */ 

	/* Render Properties for drawing hidden */
	rp_hidden.mode.polygonOffsetMode.enabled = true;
	rp_hidden.mode.polygonOffsetMode.polygonOffset.factor = 2.0;
	rp_hidden.mode.polygonOffsetMode.polygonOffset.units = 1.0;
	rp_hidden.mode.depthMaskMode.enabled = true;
	rp_hidden.mode.depthTestMode.enabled = true;
	rp_hidden.mode.depthTestMode.depthfunc.func = R_LESS;

	/* Render Properties for drawing shaded */
	rp_shaded.mode.blendMode.enabled = true;
	rp_shaded.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_shaded.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	rp_shaded.mode.normalizeMode.enabled = true;
	rp_shaded.mode.lightingMode.enabled = true;
	rp_shaded.mode.cullFaceMode.enabled = true;
	rp_shaded.mode.cullFaceMode.cullface.mode = R_BACK;
	rp_shaded.mode.depthMaskMode.enabled = true;
	rp_shaded.mode.depthTestMode.enabled = true;
	rp_shaded.mode.depthTestMode.depthfunc.func = R_LESS;

	/* Render Properties for drawing texture */
	/* alpha test */
	rp_texture.mode.alphaTestMode.enabled = true;
	rp_texture.mode.alphaTestMode.alphafunc.func = R_GREATER;
	rp_texture.mode.alphaTestMode.alphafunc.ref = 0;

	/* lighting effect */
	rp_texture.mode.lightingMode.enabled = true;

	rp_texture.mode.normalizeMode.enabled = true;

	/* color blending */
	rp_texture.mode.blendMode.enabled = true;
	rp_texture.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_texture.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	
	/* depth buffer */
	rp_texture.mode.depthMaskMode.enabled = false;

	/* depth test */
	rp_texture.mode.depthTestMode.enabled = true;
	rp_texture.mode.depthTestMode.depthfunc.func = R_EQUAL;

	/* texture 2d */
	rp_texture.mode.texture2DMode.enabled = true;

	/* set texture parameter names and parameters */
	rp_texture.mode.texture2DMode.texParameteri.pname.clear();
	rp_texture.mode.texture2DMode.texParameteri.param.clear();

	rp_texture.mode.texture2DMode.texParameteri.pname.push_back( R_TEXTURE_WRAP_S );
	rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );

	rp_texture.mode.texture2DMode.texParameteri.pname.push_back( R_TEXTURE_WRAP_T );
	rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );

	/* cull face */
	rp_texture.mode.cullFaceMode.enabled = true;
	rp_texture.mode.cullFaceMode.cullface.mode = R_BACK;
}
renderMgr::~renderMgr()
{
	delete renderPtr;
}

void renderMgr::setColor3ub( unsigned char red, unsigned char green, unsigned char blue )
{
	renderPtr->setColor3ub( red, green, blue );
}

void renderMgr::setColor3d( double red, double green, double blue )
{
	renderPtr->setColor3d( red, green, blue );
}

void renderMgr::setColor4ub( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
	renderPtr->setColor4ub( red, green, blue, alpha );
}

void renderMgr::setColor4d( double red, double green, double blue, double alpha )
{
	renderPtr->setColor4d( red, green, blue, alpha );
}

void renderMgr::getColor4d( double color[4] )
{
	renderPtr->getColor4d( color );
}

void renderMgr::setLight( int index, float* ambient, float* diffuse, float* specular, float* position )
{
	renderPtr->setLight( index, ambient, diffuse, specular, position );
}

void renderMgr::enableLight( int index )
{
	renderPtr->enableLight( index );
}

void renderMgr::disableLight( int index )
{
	renderPtr->disableLight( index );
}

void renderMgr::setLineWidth( float width )
{
	renderPtr->setLineWidth( width );
}

void renderMgr::enableLineSmooth( bool enableFlag )
{
	renderPtr->enableLineSmooth( enableFlag );
}

void renderMgr::setPointSize( float size )
{
	renderPtr->setPointSize( size );
}

void renderMgr::enablePointSmooth( bool enableFlag )
{
	renderPtr->enablePointSmooth( enableFlag );
}

void renderMgr::transform( double * tMatrix )
{
	renderPtr->transform( tMatrix );
}

void renderMgr::loadIdentity()
{
	renderPtr->loadIdentity();
}

void renderMgr::bindMatrix()
{
	renderPtr->bindMatrix();
}

void renderMgr::releaseMatrix()
{
	renderPtr->releaseMatrix();
}

void renderMgr::setMaterial( float * amb, float * diff, float * spec, float * emiss, float shine )
{
	renderPtr->setMaterial( amb, diff, spec, emiss, shine );
}

void renderMgr::draw( Primitive mode, int size, vector< double > data )
{
	renderPtr->draw( mode, size, data );
}

void renderMgr::draw( Primitive mode, int csize, vector<double> colors, int size, vector<double> data )
{
	renderPtr->draw( mode, csize, colors, size, data );
}

void renderMgr::draw( Primitive mode, float* matrix, int size, vector<double> data )
{
	renderPtr->draw( mode, matrix, size, data );
}

void renderMgr::draw( Primitive mode, float* matrix, int csize, vector<double> colors, int size, vector<double> data )
{
	renderPtr->draw( mode, matrix, csize, colors, size, data );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, int size, vector<double> data )
{
	renderPtr->draw( mode, rp, size, data );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals )
{
	renderPtr->draw( mode, rp, size, data, normals );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, int tsize, vector<double> texcoords )
{
	renderPtr->draw( mode, rp, size, data, tsize, texcoords );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords )
{
	renderPtr->draw( mode, rp, size, data, normals, tsize, texcoords );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data )
{
	renderPtr->draw( mode, rp, matrix, size, data );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals )
{
	renderPtr->draw( mode, rp, matrix, size, data, normals );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, int tsize, vector<double> texcoords )
{
	renderPtr->draw( mode, rp, matrix, size, data, tsize, texcoords );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, float* matrix, int size, vector<double> data, vector<double> normals, int tsize, vector<double> texcoords )
{
	renderPtr->draw( mode, rp, matrix, size, data, normals, tsize, texcoords );
}

void renderMgr::drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, vector<double> data )
{
	renderPtr->drawLineStipple3d( factor, pattern, mode, data );
}

void renderMgr::drawLineStipple3d( int factor, unsigned short pattern, Primitive mode, float* matrix, vector<double> data )
{
	renderPtr->drawLineStipple3d( factor, pattern, mode, matrix, data );
}

void renderMgr::createGLWindow()
{
	renderPtr->createGLWindow();
}

void renderMgr::setWindowSize( int x, int y, int width, int height )
{
	renderPtr->setWindowSize( x, y, width, height );
}

void renderMgr::setClearColor( unsigned char r, unsigned char g, unsigned char b )
{
	renderPtr->setClearColor( r, g, b );
}

void renderMgr::setProjection( double left, double right, double top, double bottom, double clipNear, double clipFar )
{
	renderPtr->setProjection( left, right , top, bottom, clipNear, clipFar );
}

void renderMgr::setBackgroundImage( float x, float y, float width, float height, float scaleW, float scaleH, unsigned char * imageData )
{
	renderPtr->setBackgroundImage( x, y, width, height, scaleW, scaleH, imageData );
}

void renderMgr::removeBackgroundImage()
{
	renderPtr->removeBackgroundImage();
}

void renderMgr::bindAttrib( RenderProperties rp )
{
	renderPtr->bindAttrib( rp );
}

void renderMgr::releaseAttrib()
{
	renderPtr->releaseAttrib();
}

SingleRenderer::SingleRenderer()
{
	renMgr = new renderMgr();
}

SingleRenderer::~SingleRenderer()
{
	delete renMgr;
}