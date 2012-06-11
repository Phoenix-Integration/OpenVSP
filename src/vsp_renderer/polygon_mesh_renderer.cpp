#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#endif

#ifdef _WIN32
#	include <windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#	include <OpenGL\glu.h>
#else
#	include <gl\GL.h>
#	include <gl\GLU.h>
#endif

#include "polygon_mesh_renderer.h"
#include "common.h"

/******************************************************
*
* Cross Section Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor. 
*
*******************************************************/
XSecSurfRenderer::XSecSurfRenderer()
{
	num_xsecs = 0;
	num_pnts = 0;

	refl_pnts_xsecs_code = NO_SYM;
	refl_hidden_code = NO_SYM;
	refl_normals_code = NO_SYM;

	highlight_xsec_flag = 0;
	highlight_xsec_id = 0;
	highlight_xsec_color = vec3d( 0.75, 0.0, 0.0 );  // default red

	fast_draw_flag = 0;
	draw_flag = true;

	numSectInterp = 0;
}

XSecSurfRenderer::~XSecSurfRenderer()
{
}

/******************************************************
*
* Set Rendering Vertices.
*
*******************************************************/
void XSecSurfRenderer::set_render_pnts( int num_xsecs_in, int num_pnts_in, array_2d<vec3d> pnts_xsecs_in )
{
	/* Set Geometry Vertices */
	num_xsecs = num_xsecs_in;
	num_pnts = num_pnts_in;
	pnts_xsecs = pnts_xsecs_in;

	/* Initialize Reflected Vertices, Normals and UW Coordinates */
	load_refl_pnts_xsecs();
	load_normals();
	load_uw();
}

/******************************************************
*
* Draw Wire Frame. 
*
*******************************************************/
void XSecSurfRenderer::draw_wire()
{
	int i, j, fast_1, fast_2;
	double dpnt[3];  // vertex - x, y, z
	double color[4];  // color - red, green, blue, alpha

	glLineWidth(1.0);  // set line width

	/* Adjust step size for faster rendering */
	fast_1 = fast_2 = 1;
	if ( fast_draw_flag )
	{
		fast_1 = MAX( MIN( 4, num_xsecs - 1 ), 1 );
		fast_2 = MAX( MIN( 4, num_pnts - 1 ), 1);

		if ( num_xsecs <= 4 ) fast_1 = 1;
		if ( num_pnts <= 4) fast_2 = 1;
	}

	/* Draw Wire Frame */
	for ( i = 0; i < num_xsecs; i += fast_1 )
	{
		/* adjust settings for highlight */
		if ( highlight_xsec_flag && highlight_xsec_id == i )
		{
			glGetDoublev( GL_CURRENT_COLOR, color );  // store current color for later use
			glColor3d( highlight_xsec_color.x(), highlight_xsec_color.y(), highlight_xsec_color.z() ); 
			glLineWidth( 3.0 );
		}

		/* draw wire */
		glBegin( GL_LINE_STRIP );
		for ( j = 0; j < num_pnts; j += fast_2 )
		{
			pnts_xsecs( i, j ).get_pnt( dpnt );
			glVertex3dv( dpnt );
		}
		glEnd();

		/* restore back to previous settings */
		if ( highlight_xsec_flag && highlight_xsec_id == i )
		{
			glColor3dv( color );
			glLineWidth ( 1.0 );
		}
	}

	/* Draw Stringers */
	for ( i = 0; i < num_pnts; i += fast_2 )
	{
		glBegin( GL_LINE_STRIP );
		for ( j = 0; j < num_xsecs; j += fast_1 )
		{
			pnts_xsecs( j, i ).get_pnt( dpnt );
			glVertex3dv( dpnt );
		}
		glEnd();
	}
}

/******************************************************
*
*  Draw Hidden Surface.
*
*******************************************************/
void XSecSurfRenderer::draw_hidden()
{
	int i, j, fast_1, fast_2;
	double dpnt[3]; // Vertex - x, y, z

   /* Draw Outline */
	draw_wire();

	glPolygonOffset( 2.0, 1 );
#ifndef __APPLE__
	glEnable( GL_POLYGON_OFFSET_EXT );
#endif
	glColor3f( 1.0f, 1.0f, 1.0f ); // white

	/* Adjust step size for faster rendering */
	fast_1 = fast_2 = 1;
	if ( fast_draw_flag )
	{
		fast_1 = MAX( MIN( 4, num_xsecs - 1), 1 );
		fast_2 = MAX( MIN( 4, num_pnts -1 ), 1 );

		if ( num_xsecs <= 4 ) fast_1 = 1;
		if ( num_pnts <= 4 ) fast_2 = 1;
	}

	/* Draw Cross Section */
	for ( i = 0; i < num_xsecs - 1; i += fast_1 )
	{
		for ( j = 0; j < num_pnts - 1; j += fast_2 )
		{
			glBegin( GL_POLYGON );
				pnts_xsecs( i, j ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				pnts_xsecs( i + 1, j ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				pnts_xsecs( i + 1, j + 1 ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				pnts_xsecs( i, j + 1 ).get_pnt( dpnt );
				glVertex3dv( dpnt );
			glEnd();
		}
	}

#ifndef __APPLE__
	glDisable( GL_POLYGON_OFFSET_EXT );
#endif
}

/******************************************************
*
* Draw Shaded Cross Section Surface. 
*
*******************************************************/
void XSecSurfRenderer::draw_shaded()
{
	int i, j;
	int fast_1, fast_2;
	float fpnt[3];  // Normal Vector - x, y, z
	double dpnt[3];  // Vertex - x, y, z

	/* Adjust step size for faster rendering */
	fast_1 = fast_2 = 1;
	if ( fast_draw_flag )
	{
		fast_1 = MAX( MIN( 4, num_xsecs - 1 ), 1 );
		fast_2 = MAX( MIN( 4, num_pnts - 1 ), 1 );

		if ( num_xsecs <= 6 ) fast_1 = 1;
		if ( num_pnts <= 6 ) fast_2 = 1;
	}

	/* Enable Shading */
	glEnable( GL_LIGHTING );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	/* Draw Cross Section */
	for ( i = 0; i < num_xsecs - 1; i += fast_1 )
	{
		for ( j = 0; j < num_pnts - 1; j += fast_2 )
		{
			glBegin( GL_POLYGON );
				normals( i, j ).get_pnt( fpnt );
				glNormal3fv( fpnt );
				pnts_xsecs( i, j ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				normals( i + 1, j ).get_pnt( fpnt );
				glNormal3fv( fpnt );
				pnts_xsecs( i + 1, j ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				normals( i + 1, j + 1 ).get_pnt( fpnt );
				glNormal3fv( fpnt );
				pnts_xsecs( i + 1, j + 1 ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				normals( i, j + 1 ).get_pnt( fpnt );
				glNormal3fv( fpnt );
				pnts_xsecs( i, j + 1 ).get_pnt( dpnt );
				glVertex3dv( dpnt );	
			glEnd();
		}
	}

	/* Diable Shading */
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
}

/******************************************************
*
* Draw Reflected Wire Frame.
*
*******************************************************/
void XSecSurfRenderer::draw_refl_wire( int sym_code_in )
{
	if ( sym_code_in == NO_SYM ) return;

	int i, j, fast_1, fast_2;
	double dpnt[3];  // vertex - x, y, z

	/* Adjust step size for faster rendering */
	fast_1 = fast_2 = 1;
	if ( fast_draw_flag )
	{
		fast_1 = MAX( MIN( 4, num_xsecs - 1 ), 1 );
		fast_2 = MAX( MIN( 4, num_pnts - 1 ), 1);

		if ( num_xsecs <= 4 ) fast_1 = 1;
		if ( num_pnts <= 4) fast_2 = 1;
	}

	/* Load Reflected Points as needed */
	if ( sym_code_in != refl_pnts_xsecs_code )
	{
		refl_pnts_xsecs_code = sym_code_in;
		load_refl_pnts_xsecs();
	}

	glLineWidth(1.0);  // set line width

	/* Draw Cross Section */
	for ( i = 0; i < num_xsecs; i += fast_1 )
	{
		glBegin( GL_LINE_STRIP );
		for ( j = 0; j < num_pnts; j += fast_2 )
		{
			refl_pnts_xsecs( i, j ).get_pnt( dpnt );
			glVertex3dv( dpnt );
		}
		glEnd();
	}

	/* Draw Stringers */
	for ( i = 0; i < num_pnts; i += fast_2 )
	{
		glBegin( GL_LINE_STRIP );
		for ( j = 0; j < num_xsecs; j += fast_1 )
		{
			refl_pnts_xsecs( j, i ).get_pnt( dpnt );
			glVertex3dv( dpnt );
		}
		glEnd();
	}
}

/******************************************************
*
* Draw Reflected Hidden Surface.
*
*******************************************************/
void XSecSurfRenderer::draw_refl_hidden( int sym_code_in )
{
	/* No Reflection */
	if ( sym_code_in == NO_SYM ) return;

	int i, j, fast_1, fast_2;
	double dpnt[3];

	/* Adjust step size for faster rendering */
	fast_1 = fast_2 = 1;
	if ( fast_draw_flag )
	{
		fast_1 = MAX( MIN( 4, num_xsecs - 1 ), 1 );
		fast_2 = MAX( MIN( 4, num_pnts - 1 ), 1);

		if ( num_xsecs <= 4 ) fast_1 = 1;
		if ( num_pnts <= 4) fast_2 = 1;
	}

	/* Load Reflected Points as needed */
	if ( sym_code_in != refl_pnts_xsecs_code )
	{
		refl_pnts_xsecs_code = sym_code_in;
		load_refl_pnts_xsecs();
	}

	/* Load Reflected Hidden Surface as needed */
	if ( sym_code_in != refl_hidden_code )
	{
		refl_hidden_code = sym_code_in;
	}

	/* Draw Outline */
	draw_refl_wire( sym_code_in );

	/* Draw Hidden Surface */
	glPolygonOffset(2.0, 1);
#ifndef __APPLE__
	glEnable( GL_POLYGON_OFFSET_EXT );
#endif

	glColor3f( 1.0f, 1.0f, 1.0f ); // white

	for ( i = 0; i < num_xsecs - 1; i += fast_1 )
	{
		for ( j = 0; j < num_pnts - 1; j += fast_2 )
		{
			glBegin( GL_POLYGON );
				refl_pnts_xsecs( i, j ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				refl_pnts_xsecs( i + 1, j ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				refl_pnts_xsecs( i + 1, j + 1 ).get_pnt( dpnt );
				glVertex3dv( dpnt );

				refl_pnts_xsecs( i, j + 1 ).get_pnt( dpnt );
				glVertex3dv( dpnt );
			glEnd();
		}
	}

#ifndef __APPLE__
	glEnable( GL_POLYGON_OFFSET_EXT );
#endif
}

/******************************************************
*
* Draw Reflected Shaded Surface.
*
*******************************************************/
void XSecSurfRenderer::draw_refl_shaded( int sym_code_in )
{
	/* No Reflection */
	if ( sym_code_in == NO_SYM ) return;

	int i, j;
	int fast_1, fast_2;
	float fpnt[3]; 
	double dpnt[3];

	/* Adjust step size for faster rendering */
	fast_1 = fast_2 = 1;
	if ( fast_draw_flag )
	{
		fast_1 = MAX( MIN( 4, num_xsecs - 1 ), 1 );
		fast_2 = MAX( MIN( 4, num_pnts - 1 ), 1);

		if ( num_xsecs <= 6 ) fast_1 = 1;
		if ( num_pnts <= 6) fast_2 = 1;
	}

	/* Load Reflected Vertices as needed */
	if ( sym_code_in != refl_pnts_xsecs_code )
	{
		refl_pnts_xsecs_code = sym_code_in;
		load_refl_pnts_xsecs();
	}

	/* Load Reflected Normals as needed */
	if ( sym_code_in != refl_normals_code )
	{
		refl_normals_code = sym_code_in;
		load_refl_normals();
	}

	/* Enable Shading */
	glEnable( GL_LIGHTING );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	/* Draw Cross Section */
	for ( i = 0; i < num_xsecs - 1; i += fast_1 )
	{
		for ( j = 0; j < num_pnts - 1; j += fast_2 )
		{
			glBegin( GL_POLYGON );
			refl_normals( i, j ).get_pnt( fpnt );
			glNormal3fv( fpnt );
			refl_pnts_xsecs( i, j ).get_pnt( dpnt );
			glVertex3dv( dpnt );

			refl_normals( i, j + 1 ).get_pnt( fpnt );
			glNormal3fv( fpnt );
			refl_pnts_xsecs( i, j + 1 ).get_pnt( dpnt );
			glVertex3dv( dpnt );

			refl_normals( i + 1, j + 1 ).get_pnt( fpnt );
			glNormal3fv( fpnt );
			refl_pnts_xsecs( i + 1, j + 1 ).get_pnt( dpnt );
			glVertex3dv( dpnt );

			refl_normals( i + 1, j ).get_pnt( fpnt );
			glNormal3fv( fpnt );
			refl_pnts_xsecs( i + 1, j ).get_pnt( dpnt );
			glVertex3dv( dpnt );
			glEnd();
		}
	}

	/* Restore Previous Settings */
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
}

/******************************************************
*
* Draw Texture.
*
*******************************************************/
void XSecSurfRenderer::draw_texture( AppliedTex& apTex )
{
	/* Make sure cross section exists */
	if ( num_xsecs <= 0 || num_pnts <= 0 )
		return;

	int i, j;

	/* Remap Texture Coordinates As Needed And Draw Texture */
	if ( apTex.repeatFlag == false )  // No repeating texturing
	{
		/* remap u dimension */
		vector< int > uIndex;
		vector< double > uVal;
		vector< double > uVec;

		for ( i = 0; i < uArray.dimension(); i++ )
			uVec.push_back( uArray( i ) );

		remap_texture( apTex.u, apTex.scaleu, apTex.wrapUFlag, uVec, uIndex, uVal );

		if ( apTex.flipUFlag )
		{
			for ( i = 0; i < ( int )uVal.size(); i++ )
				uVal[i] = 1.0 - uVal[i];
		}
		
		/* remap w dimension */
		vector< int > wIndex;
		vector< double > wVal;
		vector< double > wVec;

		for ( i = 0; i < wArray.dimension(); i++ )
			wVec.push_back( wArray(i) );

		remap_texture( apTex.w, apTex.scalew, apTex.wrapUFlag, wVec, wIndex, wVal );

		if ( apTex.flipWFlag )
		{
			for ( i = 0; i < ( int )wVal.size(); i++ )
				wVal[i] = 1.0 - wVal[i];
		}

		/* draw texture */
		for ( i = 0; i < ( int )uIndex.size(); i++ )
		{
			int iu = uIndex[i];
			int iuplus = uIndex[ i + 1 ];
			for ( j = 0; j < ( int )wIndex.size(); j++ )
			{
				int iw = wIndex[j];
				int iwplus = wIndex[ j + 1 ];
				glBegin( GL_POLYGON );
					glNormal3dv( normals(iu, iw).v );
					glTexCoord2d( uVal[i], wVal[j] );
					glVertex3dv( pnts_xsecs( iu, iw ).v );

					glNormal3dv( normals(iuplus, iw).v );
					glTexCoord2d( uVal[ i + 1 ], wVal[j] );
					glVertex3dv( pnts_xsecs( iuplus, iw ).v );

					glNormal3dv( normals(iuplus, iwplus).v );
					glTexCoord2d( uVal[ i + 1 ], wVal[ j + 1 ] );
					glVertex3dv( pnts_xsecs( iuplus, iwplus ).v );

					glNormal3dv( normals(iu, iwplus).v );
					glTexCoord2d( uVal[i], wVal[ j + 1 ] );
					glVertex3dv( pnts_xsecs( iu, iwplus ).v );
				glEnd();
			}
		}
	}
	else // Repeating texturing
	{
		/* Draw Texture */
		for ( i = 0; i < ( int )uArray.dimension() - 1; i++ )
		{
			for ( j = 0; j < ( int )wArray.dimension() - 1; j++ )
			{
				glBegin( GL_POLYGON );
					glNormal3dv( normals( i, j ).v );
					glTexCoord2d( uArray( i ) / apTex.scaleu + apTex.u, wArray( j ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i, j ).v );

					glNormal3dv( normals( i + 1, j ).v );
					glTexCoord2d( uArray( i + 1 ) / apTex.scaleu + apTex.u, wArray( j ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i + 1, j ).v );

					glNormal3dv( normals( i + 1, j + 1 ).v );
					glTexCoord2d( uArray( i + 1 ) / apTex.scaleu + apTex.u, wArray( j + 1 ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i + 1, j + 1 ).v );

					glNormal3dv( normals( i, j + 1 ).v );
					glTexCoord2d( uArray( i ) / apTex.scaleu + apTex.u, wArray( j + 1 ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i, j + 1 ).v );
				glEnd();
			}
		}
	}
}

/******************************************************
*
* Draw Reflected Texture.
*
*******************************************************/
void XSecSurfRenderer::draw_refl_texture( AppliedTex& apTex, int sym_code_in )
{
	/* Verify Request Is Vaild */
	if ( sym_code_in == NO_SYM ) return;

	if ( num_xsecs <= 0 || num_pnts <= 0 ) return;

	int i, j;

	/* Load Reflected Vertices */
	if ( sym_code_in != refl_pnts_xsecs_code )
	{
		refl_pnts_xsecs_code = sym_code_in;
		load_refl_pnts_xsecs();
	}

	/* Load Reflected Normals */
	if ( sym_code_in != refl_normals_code )
	{
		refl_normals_code = sym_code_in;
		load_refl_normals();
	}

	/* Remap Texture Coordinates and Draw Texture */
	if ( apTex.repeatFlag == false )  // No repeating texturing
	{
		/* remap u dimension */
		vector< int > uIndex;
		vector< double > uVal;
		vector< double > uVec;

		for ( i = 0; i < uArray.dimension(); i++ )
			uVec.push_back( uArray( i ) );

		remap_texture( apTex.u, apTex.scaleu, apTex.wrapUFlag, uVec, uIndex, uVal );

		if ( apTex.flipUFlag )
		{
			for ( i = 0; i < ( int )uVal.size(); i++ )
				uVal[i] = 1.0 - uVal[i];
		}
		
		/* remap w dimension */
		vector< int > wIndex;
		vector< double > wVal;
		vector< double > wVec;

		for ( i = 0; i < wArray.dimension(); i++ )
			wVec.push_back( wArray(i) );

		remap_texture( apTex.w, apTex.scalew, apTex.wrapUFlag, wVec, wIndex, wVal );

		if ( apTex.flipWFlag )
		{
			for ( i = 0; i < ( int )wVal.size(); i++ )
				wVal[i] = 1.0 - wVal[i];
		}

		/* draw texture */
		for ( i = 0; i < ( int )uIndex.size(); i++ )
		{
			int iu = uIndex[i];
			int iuplus = uIndex[ i + 1 ];
			for ( j = 0; j < ( int )wIndex.size(); j++ )
			{
				int iw = wIndex[j];
				int iwplus = wIndex[ j + 1 ];
				glBegin( GL_POLYGON );
					glNormal3dv( normals(iu, iw).v );
					glTexCoord2d( uVal[i], wVal[j] );
					glVertex3dv( pnts_xsecs( iu, iw ).v );

					glNormal3dv( normals(iuplus, iw).v );
					glTexCoord2d( uVal[ i + 1 ], wVal[j] );
					glVertex3dv( pnts_xsecs( iuplus, iw ).v );

					glNormal3dv( normals(iuplus, iwplus).v );
					glTexCoord2d( uVal[ i + 1 ], wVal[ j + 1 ] );
					glVertex3dv( pnts_xsecs( iuplus, iwplus ).v );

					glNormal3dv( normals(iu, iwplus).v );
					glTexCoord2d( uVal[i], wVal[ j + 1 ] );
					glVertex3dv( pnts_xsecs( iu, iwplus ).v );
				glEnd();
			}
		}
	}
	else // Repeating texturing
	{
		/* Draw Texture */
		for ( i = 0; i < ( int )uArray.dimension() - 1; i++ )
		{
			for ( j = 0; j < ( int )wArray.dimension() - 1; j++ )
			{
				glBegin( GL_POLYGON );
					glNormal3dv( normals( i, j ).v );
					glTexCoord2d( uArray( i ) / apTex.scaleu + apTex.u, wArray( j ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i, j ).v );

					glNormal3dv( normals( i + 1, j ).v );
					glTexCoord2d( uArray( i + 1 ) / apTex.scaleu + apTex.u, wArray( j ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i + 1, j ).v );

					glNormal3dv( normals( i + 1, j + 1 ).v );
					glTexCoord2d( uArray( i + 1 ) / apTex.scaleu + apTex.u, wArray( j + 1 ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i + 1, j + 1 ).v );

					glNormal3dv( normals( i, j + 1 ).v );
					glTexCoord2d( uArray( i ) / apTex.scaleu + apTex.u, wArray( j + 1 ) / apTex.scalew + apTex.w );
					glVertex3dv( pnts_xsecs( i, j + 1 ).v );
				glEnd();
			}
		}
	}
}

/******************************************************
*
* Load Reflected Points and Cross Section Array.
*
*******************************************************/
void XSecSurfRenderer::load_refl_pnts_xsecs()
{
	/* No Reflection */
	if ( refl_pnts_xsecs_code == NO_SYM ) return;

	int i, j;

	/* Initialize Reflected Points */
	refl_pnts_xsecs.init( num_xsecs, num_pnts );

	/* Set Reflected Points */
	if ( refl_pnts_xsecs_code == XY_SYM )
	{
		for ( i = 0; i < num_xsecs; i++ )
			for ( j = 0; j < num_pnts; j++)
				refl_pnts_xsecs( i, j ) = pnts_xsecs( i, j ).reflect_xy();
	}
	else if ( refl_pnts_xsecs_code == XZ_SYM )
	{
		for ( i = 0; i < num_xsecs; i++ )
			for ( j = 0; j < num_pnts; j++)
				refl_pnts_xsecs( i, j ) = pnts_xsecs( i, j ).reflect_xz();
	}
	else if ( refl_pnts_xsecs_code == YZ_SYM )
	{
		for ( i = 0; i < num_xsecs; i++ )
			for ( j = 0; j < num_pnts; j++)
				refl_pnts_xsecs( i, j ) = pnts_xsecs( i, j ).reflect_yz();
	}
}

/******************************************************
*
* Load Normals.
*
*******************************************************/
void XSecSurfRenderer::load_normals()
{
	normals.init( num_xsecs, num_pnts );

	int i, j;
	vec3d tan_u, tan_w;

	/* Compute Tangents */
	for ( i = 0; i < num_xsecs; i++ )
	{
		for ( j = 0; j < num_pnts; j++ )
		{
			/* compute tangent in u direction */
			if ( i == 0 )
				tan_u = pnts_xsecs( 1, j ) - pnts_xsecs( 0, j );
			else if ( i == num_xsecs - 1 )
				tan_u = pnts_xsecs( num_xsecs - 1, j ) - pnts_xsecs( num_xsecs - 2, j );
			else
				tan_u = pnts_xsecs( i + 1, j ) - pnts_xsecs( i - 1, j );

			/* compute tangent in w direction */
			if ( j == 0 || j == num_pnts - 1 )
				tan_w = pnts_xsecs( i, 1 ) - pnts_xsecs( i, num_pnts - 2 );
			else
				tan_w = pnts_xsecs( i, j + 1 ) - pnts_xsecs( i, j - 1 );

			if ( tan_w.mag() <= 0.0 )
			{
				if ( i == 0 && j == num_pnts - 1 )
					tan_w = pnts_xsecs( 1, 1 ) - pnts_xsecs( 0, j );
				else if ( i == 0 )
					tan_w = pnts_xsecs( 1, j + 1 ) - pnts_xsecs( 0, j );
				else if ( i == num_xsecs - 1 && j == num_pnts - 1 )
					tan_w = pnts_xsecs( num_xsecs - 2, 1 ) - pnts_xsecs( num_xsecs - 1 , j );
				else if ( i == num_xsecs - 1 )
					tan_w = pnts_xsecs( num_xsecs - 2, j + 1 ) - pnts_xsecs( num_xsecs - 1, j );
				else if ( j == num_pnts - 1 )
					tan_w = pnts_xsecs( i + 1, 1 ) - pnts_xsecs( i, j );
				else
					tan_w = pnts_xsecs( i + 1, j ) - pnts_xsecs( i, j + 1 );
			}
			normals( i, j ) = cross( tan_u, tan_w );
			normals( i, j ).normalize();
		}
	}
	load_refl_normals();
}

/******************************************************
*
* Load Reflected Normals.
*
*******************************************************/
void XSecSurfRenderer::load_refl_normals()
{
	/* No Reflection */
	if ( refl_normals_code == NO_SYM ) return;

	int i, j;
	vec3d sym_vec;

	/* Set Reflected Vector */
	if ( refl_normals_code == XY_SYM )
		sym_vec = vec3d( 1.0, 1.0, -1.0);
	else if ( refl_normals_code == XZ_SYM )
		sym_vec = vec3d( 1.0, -1.0, 1.0);
	else if ( refl_normals_code == YZ_SYM )
		sym_vec = vec3d( -1.0, 1.0, 1.0 );

	/* Load Normals */
	refl_normals.init(num_xsecs, num_pnts);
	for ( i = 0; i < num_xsecs; i++ )
	{
		for ( j = 0; j < num_pnts; j++ )
		{
			refl_normals( i, j ) = normals( i, j ) * sym_vec;
		}
	}
}

/******************************************************
*
* Load uw.
*
*******************************************************/
void XSecSurfRenderer::load_uw()
{
	int i, j;
	uArray.init( num_xsecs );
	wArray.init( num_pnts );

	/* Verify Request Is Vaild */
	if ( num_xsecs == 0 || num_pnts == 0 ) return;

	double total_d = 0;
	uArray( 0 ) = total_d;
	for ( i = 1; i < num_xsecs; i++ )
	{
		total_d += dist( pnts_xsecs( i, num_pnts / 2 ), pnts_xsecs( i - 1, num_pnts / 2 ) ) + 0.000001;
		uArray( i ) = total_d;
	}

	for ( i = 1; i < num_xsecs; i++ )
		uArray( i ) = uArray( i ) / total_d;

	total_d = 0;
	wArray( 0 ) = total_d;
	for ( j = 1; j < num_pnts; j++ )
	{
		total_d += dist( pnts_xsecs( num_xsecs / 2, j - 1 ), pnts_xsecs( num_xsecs / 2, j ) ) + 0.000001;
		wArray( j ) = total_d;
	}
	for ( j = 1; j < num_pnts; j++ )
		wArray( j ) = wArray( j ) / total_d;
}

/******************************************************
*
* Remap Texture Coordinates - Handle Translation / scale
* and Seams.
*
*******************************************************/
void XSecSurfRenderer::remap_texture( double uPos, double width, bool wrapFlag, vector< double > & uVec,
									vector< int > & uIndex, vector< double > & uRemap )
{
	/* Verify Request Is Vaild */
	if ( uVec.size() < 2 ) return;

	int i;

	/* Calculate U Range */
	double ulow = uPos - width / 2.0;
	double uhigh = uPos + width / 2.0;

	/* Check If 0-1 Boundray Crossed */
	int uCrossFlag = 0;
	if ( ulow < 0.0 )
	{
		ulow += -( int )( ulow - 1 );
		uCrossFlag = 1;
	}
	if ( uhigh > 1.0 )
	{
		uhigh -= ( int )uhigh;
		uCrossFlag = 1;
	}

	/* Find U Vector Just Outside Lower Bound */
	double ulowSlop = 0.0;
	int ulowIndex = 0;
	for ( i = uVec.size() - 1; i >= 0; i-- )
	{
		if ( uVec[i] < ulow )
		{
			ulowIndex = i;
			ulowSlop = ulow - uVec[i];
			break;
		}
	}

	/* Find U Vector Just Outside Upper Bound */
	double uhighSlop = 0.0;
	int uhighIndex = uVec.size() - 1;
	for ( i = 0; i < ( int ) uVec.size(); i++ )
	{
		if ( uVec[i] > uhigh )
		{
			uhighIndex = i;
			uhighSlop = uVec[i] - uhigh;
			break;
		}
	}

	/* Store U Index and U Remap Vectors */
	if ( !uCrossFlag )
	{
		for ( i = ulowIndex; i < uhighIndex; i++ )
		{
			uIndex.push_back( i );
			uRemap.push_back( uVec[i] );
		}
	}
	else
	{
		for ( i = ulowIndex; i < ( int )uVec.size(); i++ )
		{
			uIndex.push_back( i );
			uRemap.push_back( uVec[i] );
		}

		int iStart = 0;
		if ( wrapFlag )
			iStart = 1;

		for ( i = iStart; i <= uhighIndex; i++ )
		{
			uIndex.push_back( i );
			uRemap.push_back( uVec[i] + 1.0 );
		}
	}

	/* Find Total U Range */
	double totalU = 1.0;
	if ( uRemap.size() )
	{
		totalU = uRemap.back() - uRemap[0];
	}
	totalU = totalU - ulowSlop - uhighSlop;

	/* Update U Remap Vectors */
	double uZero = 0.0;
	uZero = uRemap[0] / totalU + ulowSlop / totalU;

	for ( i = 0; i < ( int )uRemap.size(); i++ )
	{
		uRemap[i] = uRemap[i] / totalU - uZero;
	}
}

/******************************************************
*
* Get Cross Section at Index.
*
*******************************************************/
void XSecSurfRenderer::get_xsec( int ixs, vector< vec3d > & pvec )
{
	pvec.resize( num_pnts );
	for ( int j = 0; j < num_pnts; j++ )
	{
		pvec[j] = pnts_xsecs( ixs, j );
	}
}

/******************************************************
*
* Get Cross Section Stringer.
*
*******************************************************/
void XSecSurfRenderer::get_stringer( int ipnt, vector< vec3d > & pvec )
{
	pvec.resize( num_xsecs );
	for ( int i = 0; i < num_xsecs; i++ )
	{
		pvec[i] = pnts_xsecs( i, ipnt );
	}
}