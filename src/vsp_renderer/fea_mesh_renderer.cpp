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

#include <assert.h>
#include "fea_mesh_renderer.h"

/******************************************************
*
* FEA Slice Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaSliceRenderer::feaSliceRenderer()
{
	highlight = false;

	/* Initialize End Points */
	vec3d zeros = vec3d( 0, 0, 0 );
	upperEndPnts.start = upperEndPnts.end = zeros;
	upperChainPnts.start = upperChainPnts.end = zeros;
	lowerChainPnts.start = lowerChainPnts.end = zeros;
}
feaSliceRenderer::~feaSliceRenderer()
{
	/* clear plane info */
	planeData.clear();
	
	/* clear upper and lower pnt info */
	upperPnts.clear();
	lowerPnts.clear();
}

/******************************************************
*
* Set Boolean Value.
*
*******************************************************/
void feaSliceRenderer::setValue( bool value, int ID )
{
	switch ( ID )
	{
		case R_FEA_SLICE_HIGHLIGHT_FLAG:
			highlight = value;
			break;
	}
}

/******************************************************
*
* Set Vector Value.
*
*******************************************************/
void feaSliceRenderer::setValue( vector< vec3d > value, int ID )
{
	switch ( ID )
	{
		/* Set planeData */
		case R_FEA_SLICE_PLANEDATA:

			/* In case planeData is not empty, clear
			   planeData before set new value */
			planeData.clear();

			for ( int i = 0; i < value.size(); i++ )
			{
				planeData.push_back( value[i] );
			}
			break;

		/* Set upperPnts */
		case R_FEA_SLICE_UPPER_PNTS:

			upperPnts.clear();
			for ( int i = 0; i < value.size(); i++ )
			{
				upperPnts.push_back( value[i] );
			}
			break;

		/* Set lowerPnts */
		case R_FEA_SLICE_LOWER_PNTS:

			lowerPnts.clear();
			for ( int i = 0; i < value.size(); i++ )
			{
				lowerPnts.push_back( value[i] );
			}
			break;
	}
}

/******************************************************
*
* Set EndPoints Value.
*
*******************************************************/
void feaSliceRenderer::setValue( EndPoints value, int ID )
{
	switch ( ID )
	{
		/* set upperEndPnts */
		case R_FEA_SLICE_UPPER_END_PNTS:
			upperEndPnts = value;
			break;

		/* set upperChainPnts */
		case R_FEA_SLICE_UPPER_CHAIN_PNTS:
			upperChainPnts = value;
			break;

		/* set lowerChainPnts */
		case R_FEA_SLICE_LOWER_CHAIN_PNTS:
			lowerChainPnts = value;
			break;
	}
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void feaSliceRenderer::reset()
{
	highlight = false;

	/* clear plane info */
	planeData.clear();
	
	/* clear upper and lower pnt info */
	upperPnts.clear();
	lowerPnts.clear();

	/* reset end points */
	vec3d zeros = vec3d( 0, 0, 0 );
	upperEndPnts.start = upperEndPnts.end = zeros;
	upperChainPnts.start = upperChainPnts.end = zeros;
	lowerChainPnts.start = lowerChainPnts.end = zeros;
}

/******************************************************
*
* Draw FEA Slice.
*
*******************************************************/
void feaSliceRenderer::draw()
{
	/* Draw Slice */
	glPointSize( 8.0 );

	/* draw outline */
	glBegin( GL_POINTS );
	for ( int i = 0; i < ( int )upperPnts.size(); i++ )
	{
		glVertex3dv( upperPnts[i].data() );
	}
	for ( int i = 0; i < ( int )lowerPnts.size(); i++ )
	{
		glVertex3dv( lowerPnts[i].data() );
	}
	glEnd();

	/* draw inner line */
	glBegin( GL_LINES );
	for ( int i = 0; i < ( int )upperPnts.size(); i++ )
	{
		glVertex3dv( upperPnts[i].data() );
		glVertex3dv( lowerPnts[i].data() );
	}
	glEnd();
}

/******************************************************
*
* Draw Slice Plane.
*
*******************************************************/
void feaSliceRenderer::drawSlicePlane()
{
	/* Hard coded index, must make
	   sure planeData has a vaild size */
	assert( planeData.size() == 4 );
	if ( planeData.size() != 4 )
		return;

	/* Draw Slice Plane */
	glBegin( GL_LINES );
	for ( int i = 0; i < 4; i++ )
	{
		double fu = ( double )i / 3.0;
		vec3d p0 = planeData[0] + ( planeData[1] - planeData[0] ) * fu;
		vec3d p1 = planeData[3] + ( planeData[2] - planeData[3] ) * fu;
		glVertex3dv( p0.data() );
		glVertex3dv( p1.data() );
	}
	glEnd();

	glBegin( GL_LINES );
	for ( int i = 0; i < 4; i++ )
	{
		double fw = ( double )i / 3.0;
		vec3d p0 = planeData[0] + ( planeData[3] - planeData[0] ) * fw;
		vec3d p1 = planeData[1] + ( planeData[2] - planeData[1] ) * fw;
		glVertex3dv( p0.data() );
		glVertex3dv( p1.data() );
	}
	glEnd();
}

/******************************************************
*
* FEA Rib Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaRibRenderer::feaRibRenderer() : feaSliceRenderer()
{
}
feaRibRenderer::~feaRibRenderer()
{
}

/******************************************************
*
* Draw FEA Rib.
*
*******************************************************/
void feaRibRenderer::draw()
{
	/* Disable smooth line & point */
	GLboolean smooth_flag = glIsEnabled( GL_LINE_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POINT_SMOOTH );

	if ( highlight )
	{
		glColor3ub( 255, 0, 0 );
		glLineWidth( 3.0 );
		drawSlicePlane();
	}
	else
	{
		glColor3ub( 150, 100, 150 );
		glLineWidth( 2.0 );

		glBegin( GL_LINES );
		glVertex3dv( upperEndPnts.start.data() );
		glVertex3dv( upperEndPnts.end.data() );
		glEnd();
	}

	/* restore to previous settings if needed */
	if ( smooth_flag )
	{
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
	}
}

/******************************************************
*
* FEA Spar Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaSparRenderer::feaSparRenderer() : feaSliceRenderer()
{
}
feaSparRenderer::~feaSparRenderer()
{
}

/******************************************************
*
* Draw FEA Spar.
*
*******************************************************/
void feaSparRenderer::draw()
{
	/* Disable smooth line & point */
	GLboolean smooth_flag = glIsEnabled( GL_LINE_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POINT_SMOOTH );

	if ( highlight )
	{
		glColor3ub( 255, 0, 0 );
		glLineWidth( 3.0 );
		drawSlicePlane();
	}
	else
	{
		glColor3ub( 100, 150, 100 );
		glLineWidth( 2.0 );
		glBegin( GL_LINES );
		vec3d p0 = ( upperChainPnts.start + lowerChainPnts.start ) * 0.5;
		vec3d p1 = ( upperChainPnts.end + lowerChainPnts.end ) * 0.5;
		glVertex3dv( p0.data() );
		glVertex3dv( p1.data() );
		glEnd();
	}

	/* restore to previous settings if needed */
	if ( smooth_flag )
	{
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
	}
}

/******************************************************
*
* FEA Wing Section Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaWingRenderer::feaWingRenderer()
{
	highlight = false;
}
feaWingRenderer::~feaWingRenderer()
{
	corners.clear();
}

/******************************************************
*
* Set bool value.
*
*******************************************************/
void feaWingRenderer::setValue( bool value, int ID )
{
	switch ( ID )
	{
		case R_FEA_WING_HIGHLIGHT_FLAG:
			highlight = value;
			break;
	}
}

/******************************************************
*
* Set vector value.
*
*******************************************************/
void feaWingRenderer::setValue( vector< vec3d > value, int ID )
{
	switch ( ID )
	{
		case R_FEA_WING_CORNERS:
			if ( value.size() != 4 )
			{
				break;
			}
			corners.clear();
			for ( int i = 0; i < value.size(); i++ )
			{
				corners.push_back( value[i] );
			}
			break;
	}
}

/******************************************************
*
* Reset to default.
*
*******************************************************/
void feaWingRenderer::reset()
{
	highlight = false;
	corners.clear();
}

/******************************************************
*
* Draw FEA Wing Section.
*
*******************************************************/
void feaWingRenderer::draw()
{
	/* Disable smooth line & point */
	GLboolean smooth_flag = glIsEnabled( GL_LINE_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POINT_SMOOTH );

	/* Set color */
	if ( highlight )
	{
		glColor4ub( 220, 220, 220, 240 );
	}
	else
	{
		glColor4ub( 170, 170, 170, 170 );
	}

	/* Draw Wing Section */
	glBegin( GL_POLYGON );
	glVertex3dv( corners[0].data() );
	glVertex3dv( corners[1].data() );
	glVertex3dv( corners[2].data() );
	glVertex3dv( corners[3].data() );
	glEnd();

	/* restore to previous settings if needed */
	if ( smooth_flag )
	{
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
	}
}

/******************************************************
*
* FEA Splice Line Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaSpliceLineRenderer::feaSpliceLineRenderer()
{
	winHeight = winWidth = 0;
	endPoints.start = 0;
	endPoints.end = 0;
}
feaSpliceLineRenderer::~feaSpliceLineRenderer()
{
}

/******************************************************
*
* Set EndPoints Value.
*
*******************************************************/
void feaSpliceLineRenderer::setValue( EndPoints value, int ID )
{
	switch ( ID )
	{
		case R_FEA_SPLICELINE_ENDPOINTS:
			endPoints = value;
			break;
	}
}

/******************************************************
*
* Set vector Value.
*
*******************************************************/
void feaSpliceLineRenderer::setValue( vector< double > value, int ID )
{
	switch ( ID )
	{
		case R_FEA_SPLICELINE_VEC_POS:

			spliceVecPos.clear();

			for ( int i = 0; i < value.size(); i++ )
			{
				spliceVecPos.push_back( value[i] );
			}
			break;
	}
}

/******************************************************
*
* Set double Value.
*
*******************************************************/
void feaSpliceLineRenderer::setValue( double value, int ID )
{
	switch ( ID )
	{
		case R_FEA_SPLICELINE_WIN_HEIGHT:
			winHeight = value;
			break;

		case R_FEA_SPLICELINE_WIN_WIDTH:
			winWidth = value;
			break;

		case R_FEA_SPLICELINE_WINXSCALE:
			winXScale = value;
			break;
	}
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void feaSpliceLineRenderer::reset()
{
	spliceVecPos.clear();
	winHeight = winWidth = 0;
	endPoints.start = 0;
	endPoints.end = 0;
}

/******************************************************
*
* Draw Grid.
*
*******************************************************/
void feaSpliceLineRenderer::drawGrid()
{
	float gridSize = 0.1f;

	/* Draw Grid */
	glLineWidth( 1.0 );
	glColor3f( 0.8f, 0.8f, 0.8f );
	glBegin( GL_LINES );
	for ( int i = 0; i < 41; i++ )
	{
		if ( i == 20 )
			glColor3f( 0.8f, 0.8f, 0.8f );
		else
			glColor3f( 0.9f, 0.9f, 0.9f );

		glVertex2f( gridSize * ( float )i - 20.0f * gridSize, -20.0f * gridSize );
		glVertex2f( gridSize * ( float )i - 20.0f * gridSize, 20.0f * gridSize );
		glVertex2f( -20.0f * gridSize, gridSize * ( float )i - 20.0f * gridSize );
		glVertex2f( 20.0f * gridSize, gridSize * ( float )i - 20.0f * gridSize );
	}
	glEnd();
}

/******************************************************
*
* Draw Control Point.
*
*******************************************************/
void feaSpliceLineRenderer::drawCtrlPnt( double pos )
{
	double ht = 0.35;
	glColor3f( 0.6f, 0.6f, 0.6f );
	drawCtrlPntHelper( pos, ht );
}

/******************************************************
*
* Draw Control Point ( Edit ).
*
*******************************************************/
void feaSpliceLineRenderer::drawCtrlPntEdit( double pos )
{
	double ht = 0.45;
	glColor3f( 1.0f, 0.0f, 0.0f );
	drawCtrlPntHelper( pos, ht );
}

/******************************************************
*
* Draw Control Point ( HighLight ).
*
*******************************************************/
void feaSpliceLineRenderer::drawCtrlPntHighlight( double pos )
{
	double ht = 0.40;
	glColor3f( 0.3f, 0.3f, 0.3f );
	drawCtrlPntHelper( pos, ht );
}

/******************************************************
*
* Draw Thick Dist.
*
*******************************************************/
void feaSpliceLineRenderer::drawThick( double currPos, double nextPos, double tRatio0, double tRatio1 )
{
	double ar = ( double ) winHeight / ( double ) winWidth;

	double t0 = tRatio0 * ar * 0.5;
	double t1 = tRatio1 * ar * 0.5;

	glColor4f( 0.0f, 0.0f, 1.0f, 1.0f );
	glLineWidth( 2.0 );
	glEnable( GL_LINE_SMOOTH );

	glBegin( GL_LINE_LOOP );
	glVertex2d( winXScale * currPos - winXScale * 0.5, t0 * 0.5 );
	glVertex2d( winXScale * currPos - winXScale * 0.5, -t0 * 0.5 );
	glVertex2d( winXScale * nextPos - winXScale * 0.5, -t1 * 0.5 );
	glVertex2d( winXScale * nextPos - winXScale * 0.5, t1 * 0.5 );
	glEnd();

	glDisable( GL_LINE_SMOOTH );
}

/******************************************************
*
* Draw Main.
*
*******************************************************/
void feaSpliceLineRenderer::drawMain()
{
	glBegin( GL_LINES );
	glVertex3dv( endPoints.start.data() );
	glVertex3dv( endPoints.end.data() );
	glEnd();

	glPointSize( 8.0 );
	glBegin( GL_POINTS );
	for ( int i = 0; i < ( int )spliceVecPos.size(); i++ )
	{
		vec3d p = endPoints.start + ( endPoints.end - endPoints.start ) * spliceVecPos[i];
		glVertex3dv( p.data() );
	}
	glEnd();
}

/******************************************************
*
* Helper Class for Rendering Control Point.
*
*******************************************************/
void feaSpliceLineRenderer::drawCtrlPntHelper( double sPos, double ht )
{
	double ar = ( double ) winHeight / ( double ) winWidth;

	glLineWidth( 1.0 );
	glPointSize( 5.0 );

	glBegin( GL_LINES );
	glVertex2d( winXScale * sPos - winXScale * 0.5, ar * ht );
	glVertex2d( winXScale * sPos - winXScale * 0.5, -ar * ht );
	glEnd();

	glBegin( GL_POINTS );
	glVertex2d( winXScale * sPos - winXScale * 0.5, ar * ht );
	glEnd();
}

/******************************************************
*
* FEA Skin Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaSkinRenderer::feaSkinRenderer()
{
	highlight = false;
}
feaSkinRenderer::~feaSkinRenderer()
{
}

/******************************************************
*
* Set bool Value.
*
*******************************************************/
void feaSkinRenderer::setValue( bool value, int ID )
{
	switch ( ID )
	{
		case R_FEA_SPLICELINE_SKIN_HIGHLIGHT:
			highlight = value;
			break;
	}
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void feaSkinRenderer::reset()
{
	highlight = false;
}

/******************************************************
*
* Draw Skin.
* This draw only sets up skin color and line width.
*
*******************************************************/
void feaSkinRenderer::draw()
{
	if ( highlight )
	{
		glColor3ub( 255, 0, 0 );
		glLineWidth( 4.0 );
	}
	else
	{
		glColor3ub( 0, 0, 255 );
		glLineWidth( 2.0 );
	}
}

/******************************************************
*
* FEA Element Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaElementRenderer::feaElementRenderer()
{
}
feaElementRenderer::~feaElementRenderer()
{
}

/******************************************************
*
* Set vector Value.
*
*******************************************************/
void feaElementRenderer::setValue( vector< vec3d > value, int ID )
{
	switch ( ID )
	{
		case R_FEA_ELEMENT_CORNER_PNTS:
			cornerPnts.clear();
			for ( int i = 0; i < value.size(); i++ )
			{
				cornerPnts.push_back( value[i] );
			}
			break;
	}
}

/******************************************************
*
* Set Color.
*
*******************************************************/
void feaElementRenderer::set_Color( unsigned char red,
												unsigned char green,
												unsigned char blue,
												unsigned char alpha )
{
	glColor4ub( red, green, blue, alpha );
}

/******************************************************
*
* Set Point Size.
*
*******************************************************/
void feaElementRenderer::set_Point_Size( double size )
{
	glPointSize( size );
}

/******************************************************
*
* Set Line Width.
*
*******************************************************/
void feaElementRenderer::set_Line_Width( double size )
{
	glLineWidth( size );
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void feaElementRenderer::reset()
{
	cornerPnts.clear();
}

/******************************************************
*
* Enable Cull Face.
*
*******************************************************/
void feaElementRenderer::enableCullFace( CullingMode mode )
{
	switch ( mode )
	{
		case FRONT:
			glCullFace( GL_FRONT );
			break;
			
		case BACK:
			glCullFace( GL_BACK );
			break;

		case FRONT_AND_BACK:
			glCullFace( GL_FRONT_AND_BACK );
			break;
	}
	glEnable( GL_CULL_FACE );
}

/******************************************************
*
* Disable Cull Face.
*
*******************************************************/
void feaElementRenderer::disableCullFace()
{
	glDisable( GL_CULL_FACE );
}

/******************************************************
*
* Draw Element Mesh.
*
*******************************************************/
void feaElementRenderer::draw()
{
	glBegin( GL_POLYGON );
	for ( int i = 0; i < ( int )cornerPnts.size(); i++ )
	{
		glVertex3dv( cornerPnts[i].data() );
	}
	glEnd();
}

/******************************************************
*
* Draw Element Wire.
*
*******************************************************/
void feaElementRenderer::drawWire()
{
	/* Disable smooth line & point */
	GLboolean smooth_flag = glIsEnabled( GL_LINE_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POINT_SMOOTH );

	glBegin( GL_LINE_LOOP );
	for ( int i = 0; i < ( int )cornerPnts.size(); i++ )
	{
		glVertex3dv( cornerPnts[i].data() );
	}
	glEnd();

	/* restore to previous settings if needed */
	if ( smooth_flag )
	{
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
	}
}

/******************************************************
*
* Point Mass Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaPointMassRenderer::feaPointMassRenderer()
{
	highlight = false;
	
   vec3d zeros = vec3d( 0, 0, 0 );
	pos = zeros;
	attachPos = zeros;
}
feaPointMassRenderer::~feaPointMassRenderer()
{
}

/******************************************************
*
* Set bool Value.
*
*******************************************************/
void feaPointMassRenderer::setValue( bool value, int ID )
{
	switch ( ID )
	{
		case R_FEA_POINTMASS_HIGHLIGHT:
			highlight = value;
			break;
	}
}

/******************************************************
*
* Set vec3d Value.
*
*******************************************************/
void feaPointMassRenderer::setValue( vec3d value, int ID )
{
	switch ( ID )
	{
		case R_FEA_POINTMASS_POS:
			pos = value;
			break;

		case R_FEA_POINTMASS_ATTACHPOS:
			attachPos = value;
			break;
	}
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void feaPointMassRenderer::reset()
{
	highlight = false;

	vec3d zeros = vec3d( 0, 0, 0 );
	pos = zeros;
	attachPos = zeros;
}

/******************************************************
*
* Draw Point Mass.
*
*******************************************************/
void feaPointMassRenderer::draw()
{
	/* Disable smooth line & point */
	GLboolean smooth_flag = glIsEnabled( GL_LINE_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POINT_SMOOTH );

	if ( highlight )
		glColor3ub( 255, 0, 0 );
	else
		glColor3ub( 100, 100, 100 );

	glLineWidth( 2.0 );
	glPointSize( 6.0 );
	glBegin( GL_LINES );
	glVertex3dv( pos.data() );
	glVertex3dv( attachPos.data() );
	glEnd();

	glBegin( GL_POINTS );
	glVertex3dv( attachPos.data() );
	glEnd();

	glPointSize( 12.0 );
	glColor3ub( 0, 0, 0 );
	glBegin( GL_POINTS );
	glVertex3dv( pos.data() );
	glEnd();

	/* restore to previous settings if needed */
	if ( smooth_flag )
	{
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
	}
}

/******************************************************
*
* Attachment Points Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaAttachmentPointsRenderer::feaAttachmentPointsRenderer()
{
	closestAttachPointIndex = -1;
}
feaAttachmentPointsRenderer::~feaAttachmentPointsRenderer()
{
}

/******************************************************
*
* Set vector< vec3d > Value.
*
*******************************************************/
void feaAttachmentPointsRenderer::setValue( vector< vec3d > value, int ID )
{
	switch ( ID )
	{
		case R_FEA_ATTACHPOINTS_VEC:
			attachPoints.clear();
			for ( int i = 0; i < ( int )value.size(); i++ )
			{
				attachPoints.push_back( value[i] );
			}
			break;
	}
}

/******************************************************
*
* Set int Value.
*
*******************************************************/
void feaAttachmentPointsRenderer::setValue( int value, int ID )
{
	switch ( ID )
	{
		case R_FEA_ATTACHPOINTS_CLOSESTATTACHPOINT_INDEX:
			closestAttachPointIndex = value;
			break;
	}
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void feaAttachmentPointsRenderer::reset()
{
	attachPoints.clear();
	closestAttachPointIndex = -1;
}

/******************************************************
*
* Draw Attachment Points.
*
*******************************************************/
void feaAttachmentPointsRenderer::draw()
{
	/* Disable smooth line & point */
	GLboolean smooth_flag = glIsEnabled( GL_LINE_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POINT_SMOOTH );

	glPointSize( 6.0 );
	glColor4ub( 100, 100, 100, 255 );
	glBegin( GL_POINTS );
	for ( int i = 0; i < ( int )attachPoints.size(); i++ )
	{
		if ( i != closestAttachPointIndex )
		{
			glVertex3dv( attachPoints[i].data() );
		}
	}
	glEnd();

	if ( closestAttachPointIndex >= 0 && closestAttachPointIndex < ( int )attachPoints.size() )
	{
		glPointSize( 8.0 );
		glColor4ub( 255, 0, 0, 255 );
		glBegin( GL_POINTS );
		glVertex3dv( attachPoints[closestAttachPointIndex].data() );
		glEnd();
	}

	/* restore to previous settings if needed */
	if ( smooth_flag )
	{
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
	}
}