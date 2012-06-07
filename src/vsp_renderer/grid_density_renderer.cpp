#include "grid_density_renderer.h"
#include "common.h"

/******************************************************
*
* Base Source Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
sourceRenderer::sourceRenderer()
{
	color.red = 255;		// default white
	color.green = 255;
	color.blue = 255;
	color.alpha = 255;
}
sourceRenderer::~sourceRenderer()
{
}

/******************************************************
*
* Base Draw Sphere.
*
*******************************************************/
void sourceRenderer::draw_sphere( double rad, const vec3d& loc )
{
	int i, j;
	int num_lats = 8;
	int num_longs = 8;

	for ( i = 0; i < num_lats; i++ )
	{
		glBegin( GL_LINE_LOOP );
		double lat = PI * ( -0.5 + (double)i/num_lats );
		double z = rad * sin( lat );
		double zr = rad * cos( lat );

		for ( j = 0; j < num_longs; j++ )
		{
			double lng = 2 * PI * (double)j/num_longs;
			double x = cos( lng ) * zr;
			double y = sin( lng ) * zr;
			glVertex3d( x + loc[0], y + loc[1], z + loc[2] );
		}
		glEnd();
	}
}

/******************************************************
*
* Set Color.
*
*******************************************************/
void sourceRenderer::set_color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
	color.red = red;
	color.green = green;
	color.blue = blue;
	color.alpha = alpha;
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void sourceRenderer::reset()
{
	color.red = 255;
	color.green = 255;
	color.blue = 255;
	color.alpha = 255;
}

/******************************************************
*
* Point Source Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor.
*
*******************************************************/
pSourceRenderer::pSourceRenderer() : sourceRenderer()
{
	m_rad = 0;
	m_loc = vec3d( 0.0, 0.0, 0.0 );
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void pSourceRenderer::reset()
{
	sourceRenderer::reset();

	m_rad = 0;
	m_loc = vec3d( 0.0, 0.0, 0.0 );
}

/******************************************************
*
* Overloading draw() From baseSourceRenderer.
* Draw Point Source.
*
*******************************************************/
void pSourceRenderer::draw()
{
	glPushMatrix();
	glLineWidth( 1.0 );
	glColor4ub( color.red, color.green, color.blue, color.alpha );
	draw_sphere( m_rad, m_loc );
	glPopMatrix();
}

/******************************************************
*
* Line Source Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor.
*
*******************************************************/
lSourceRenderer::lSourceRenderer() : sourceRenderer()
{
	m_rad1 = m_rad2 = 0;
	m_loc1 = m_loc2 = vec3d( 0.0, 0.0, 0.0 );
}

/******************************************************
*
* Set End Points.
*
*******************************************************/
void lSourceRenderer::set_end_pnts( const vec3d &value, const vec3d &value2 )
{
	m_loc1 = value;
	m_loc2 = value2;
}

/******************************************************
*
* Set Radius.
*
*******************************************************/
void lSourceRenderer::set_rad( double value, double value2 )
{
	m_rad1 = value;
	m_rad2 = value2;
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void lSourceRenderer::reset()
{
	sourceRenderer::reset();

	m_rad1 = m_rad2 = 0;
	m_loc1 = m_loc2 = vec3d( 0.0, 0.0, 0.0 );
}

/******************************************************
*
* Overloading draw() From baseSourceRenderer.
* Draw Line Source.
*
*******************************************************/
void lSourceRenderer::draw()
{
	glLineWidth( 1.0 );
	glColor4ub( color.red, color.green, color.blue, color.alpha );
	vec3d p;

	draw_sphere( m_rad1, m_loc1 );
	draw_sphere( m_rad2, m_loc2 );

	glBegin( GL_LINES );
		glVertex3d( m_loc1[0], m_loc1[1], m_loc1[2] + m_rad1 );
		glVertex3d( m_loc2[0], m_loc2[1], m_loc2[2] + m_rad2 );
		glVertex3d( m_loc1[0], m_loc1[1], m_loc1[2] - m_rad1 );
		glVertex3d( m_loc2[0], m_loc2[1], m_loc2[2] - m_rad2 );
		glVertex3d( m_loc1[0], m_loc1[1] + m_rad1, m_loc1[2] );
		glVertex3d( m_loc2[0], m_loc2[1] + m_rad2, m_loc2[2] );
		glVertex3d( m_loc1[0], m_loc1[1] - m_rad1, m_loc1[2] );
		glVertex3d( m_loc2[0], m_loc2[1] - m_rad2, m_loc2[2] );
		glVertex3d( m_loc1[0] + m_rad1, m_loc1[1], m_loc1[2] );
		glVertex3d( m_loc2[0] + m_rad2, m_loc2[1], m_loc2[2] );
		glVertex3d( m_loc1[0] - m_rad1, m_loc1[1], m_loc1[2] );
		glVertex3d( m_loc2[0] - m_rad2, m_loc2[1], m_loc2[2] );
	glEnd();
}

/******************************************************
*
* Box Source Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor.
*
*******************************************************/
bSourceRenderer::bSourceRenderer() : sourceRenderer()
{
	m_box.update( vec3d( 0.0001, 0.0001, 0.0001 ) );
	m_box.update( vec3d( -0.0001, -0.0001, -0.0001 ) );
}

/******************************************************
*
* Reset to Default.
*
*******************************************************/
void bSourceRenderer::reset()
{
	sourceRenderer::reset();
}

/******************************************************
*
* Overloading draw() From baseSourceRenderer.
* Draw Box Source.
*
*******************************************************/
void bSourceRenderer::draw()
{
	glLineWidth( 1.0 );
	glColor4ub( color.red, color.green, color.blue, color.alpha );
	glBegin( GL_LINE_LOOP );
		glVertex3dv( m_box.get_pnt( 0 ).data() );
		glVertex3dv( m_box.get_pnt( 1 ).data() );
		glVertex3dv( m_box.get_pnt( 3 ).data() );
		glVertex3dv( m_box.get_pnt( 2 ).data() );
	glEnd();

	glBegin( GL_LINE_LOOP );
		glVertex3dv( m_box.get_pnt( 4 ).data() );
		glVertex3dv( m_box.get_pnt( 5 ).data() );
		glVertex3dv( m_box.get_pnt( 7 ).data() );
		glVertex3dv( m_box.get_pnt( 6 ).data() );
	glEnd();

	glBegin( GL_LINE );
		glVertex3dv( m_box.get_pnt(0).data() );
		glVertex3dv( m_box.get_pnt(4).data() );
		glVertex3dv( m_box.get_pnt(1).data() );
		glVertex3dv( m_box.get_pnt(5).data() );
		glVertex3dv( m_box.get_pnt(3).data() );
		glVertex3dv( m_box.get_pnt(7).data() );
		glVertex3dv( m_box.get_pnt(2).data() );
		glVertex3dv( m_box.get_pnt(6).data() );
	glEnd();
}