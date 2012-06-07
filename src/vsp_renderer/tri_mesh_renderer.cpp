#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#endif

#ifdef _WIN32
#	include <Windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#else
#	include <gl\GL.h>
#endif

#include "tri_mesh_renderer.h"
#include "common.h"

/******************************************************
*
* Triangle Mesh Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor. 
*
*******************************************************/
tMeshRenderer::tMeshRenderer()
{
}
tMeshRenderer::~tMeshRenderer()
{
}

/******************************************************
*
* Draw Tirangle Mesh. 
*
*******************************************************/
void tMeshRenderer::draw()
{
	int i;
	for ( i = 0; i < ( int )vertVec.size(); i+=3 )
	{
		glBegin( GL_TRIANGLES );
		glVertex3dv( vertVec[i].data() );
		glVertex3dv( vertVec[i + 1].data() );
		glVertex3dv( vertVec[i + 2].data() );
		glEnd();
	}
}

/******************************************************
*
* Draw Wire. 
*
*******************************************************/
void tMeshRenderer::draw_wire()
{
	int i;
	for ( i = 0; i < ( int )vertVec.size(); i+=3 )
	{
		glBegin( GL_LINE_LOOP );
		glVertex3dv( vertVec[i].data() );
		glVertex3dv( vertVec[i + 1].data() );
		glVertex3dv( vertVec[i + 2].data() ); 
		glEnd();
	}
}

/******************************************************
*
* Draw Mesh with Lighting Effect. 
*
*******************************************************/
void tMeshRenderer::draw_shaded()
{
	/* Not Implement Yet */
}

/******************************************************
*
* Set Vertex Info. 
*
*******************************************************/
void tMeshRenderer::setValue( vector< vec3d > value, int ID )
{
	switch( ID )
	{
		case R_TMESH_VERTICES:
			vertVec = value;
			break;
	}
}