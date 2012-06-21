#include "renderMgr.h"

renderMgr::renderMgr()
{
	renderPtr = new IRenderer();
}
renderMgr::~renderMgr()
{
	delete renderPtr;
}

void renderMgr::init()
{
	int renderFlag = 1;
	if ( renderFlag )
	{
		delete renderPtr;
		renderPtr = new GL11Renderer();
	}
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

void renderMgr::setLineWidth( float width )
{
	renderPtr->setLineWidth( width );
}

void renderMgr::setPointSize( float size )
{
	renderPtr->setPointSize( size );
}

void renderMgr::draw( Primitive mode, int size, vector< double > data )
{
	renderPtr->draw( mode, size, data );
}

void renderMgr::draw( Primitive mode, int size, float* matrix, vector<double> data )
{
	renderPtr->draw( mode, size, matrix, data );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, int size, vector<double> data )
{
	renderPtr->draw( mode, rp, size, data );
}

void renderMgr::draw( Primitive mode, RenderProperties rp, int size, vector<double> data, vector<double> normals )
{
	renderPtr->draw( mode, rp, size, data, normals );
}
