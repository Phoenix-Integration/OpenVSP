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
#endif

#include "cfd_mesh_renderer.h"
#include "common.h"

/******************************************************
*
* CFD Mesh Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
cfdMeshRenderer::cfdMeshRenderer()
{
}
cfdMeshRenderer::~cfdMeshRenderer()
{
}

/******************************************************
*
* Set tMeshRenderer Vector.
*
*******************************************************/
void cfdMeshRenderer::setValue( vector< tMeshRenderer* > value, int ID )
{
	switch( ID )
	{
		case R_CFD_TMESH_RENDERS:
			surfaces = value;
			break;
	}
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void cfdMeshRenderer::reset()
{
	surfaces.clear();
}

/******************************************************
*
* Draw CFD MESH.
*
*******************************************************/
void cfdMeshRenderer::draw()
{
	glLineWidth( 1.0 );
	glColor4ub( 255, 0, 0, 255 );

	glPolygonOffset( 2.0, 1 );

	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

#ifndef __APPLE__
	glEnable( GL_POLYGON_OFFSET_EXT );
#endif
	glColor4ub( 220, 220, 220, 255 );
	for ( int i = 0; i < (int)surfaces.size(); i++ )
	{
		surfaces[i]->draw();
	}

	glLineWidth( 1.0 );
	glColor4ub( 100, 0, 100, 255 );
	for ( int i = 0; i < (int)surfaces.size(); i++ )
	{
		surfaces[i]->draw_wire();
	}
	glDisable( GL_CULL_FACE );

#ifndef __APPLE__
	glDisable( GL_POLYGON_OFFSET_EXT );
#endif
}