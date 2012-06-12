
// TODO:  Texture code is not working right now.
// TODO:  Need to implement Fuselage Geometries renderer.
// TODO:  Need to implement User Created Geometry renderer.

// FIXME: Hard coded primary surface for FuselageGeom, need implement something to identify surfaces. 
// FIXME: mwing surface is hard coded as first item in the vector( see WingGeom.cpp: getXSectionSurfVec() ) . 
// Need implement something to identify cross section surfaces in vector.

//#include <FL/Fl.H>  // Fuselage Geometry requires interaction with GUI.

#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#endif

#ifdef _WIN32
#	include <Windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#	include <OpenGL\glu.h>
#else
#	include <gl\GL.h>
#	include <gl\GLU.h>
#endif

#include "geom_renderer.h"
#include "material.h"

/******************************************************
*
* General Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
Renderer::Renderer()
{
	/* Initialization */
	materialID = 0;
	sym_code = NO_SYM;
	redFlag = yellowFlag = noshowFlag = 0;
	displayFlag = GEOM_WIRE_FLAG;
	color = vec3d( 1, 1, 1 );  // default white

	static float identity_mat[16] =
		{ 1.0, 0.0, 0.0, 0.0,
		  0.0, 1.0, 0.0, 0.0,
		  0.0, 0.0, 1.0, 0.0,
		  0.0, 0.0, 0.0, 1.0 };

	/* Set defalut model_mat and reflect_mat to Identity Matrix */
	model_mat = identity_mat;
	reflect_mat = identity_mat;

	/* Set Some Reasonable Values For Bounding Box */
	bnd_box_xform.update( vec3d( 0.0001, 0.0001, 0.0001 ) );
	bnd_box_xform.update( vec3d( -0.0001, -0.0001, -0.0001 ) );
}

Renderer::~Renderer()
{
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void Renderer::draw()
{
	int i;

	/* FastDraw Flag */
	for ( i = 0; i < (int)surfVec.size(); i++ )
	{
		if ( fastDrawFlag )
		{
			surfVec[i]->fast_draw_on();
		}
		else
		{
			surfVec[i]->fast_draw_off();
		}
	}

	/* Draw Highlighting Boxes */
	draw_highlight_boxes();

	/* Check Noshow Flag */
	if ( noshowFlag ) return;	

	if ( displayFlag == GEOM_WIRE_FLAG )
	{
		/* Draw Geom Wire */
		draw_wire();
		
		/* Draw Reflected Geom Wire */
		draw_wire_refl();
	}
	else if ( displayFlag == GEOM_SHADE_FLAG )
	{
		/* Draw Geom */
		draw_shade();

		/* Reflected Geom */
		draw_shade_refl();
	}
	else if ( displayFlag == GEOM_TEXTURE_FLAG )
	{
		/* Draw Texture */
		draw_textures();

		/* Reflected Texture */
		draw_textures_refl();
	}
	else if ( displayFlag == GEOM_HIDDEN_FLAG )
	{
		/* Draw Geom */
		draw_hidden();

		/* Reflected Geom */
		draw_hidden_refl();
	}
}

/******************************************************
*
* Draw Geometry With Transparent Effect.
*
*******************************************************/
void Renderer::draw_alpha()
{
	int i;
	Material* mat = matMgrPtr->getMaterial( materialID );
	if ( !mat )
		return;

	if ( mat->diff[3] > 0.99 )
		return;

	if ( noshowFlag ) return;	

	/* Update FastDraw Flag */
	for ( i = 0; i < (int)surfVec.size(); i++ )
	{
		if ( fastDrawFlag )
		{
			surfVec[i]->fast_draw_on();
		}
		else
		{
			surfVec[i]->fast_draw_off();
		}
	}

	if ( displayFlag == GEOM_SHADE_FLAG || displayFlag == GEOM_TEXTURE_FLAG )
	{
		/* Bind Material */
		mat->bind();

		/* Draw Geom */
		draw_shade();
		if ( displayFlag == GEOM_TEXTURE_FLAG )
		{
			draw_textures();
		}

		/* Draw Reflected Geom */
		draw_shade_refl();
		if ( displayFlag == GEOM_TEXTURE_FLAG )
		{
			draw_textures_refl();
		}
	}
}

/******************************************************
*
* Draw Geometry Texture.
*
*******************************************************/
void Renderer::draw_textures()
{
	/* Check if material exist */
	Material* mat = matMgrPtr->getMaterial( materialID );
	if ( !mat )
		return;

	/* Check if diffuse light is vaild */
	if ( mat->diff[3] <= 0.99 )
		return;

	/* Bind Material */
	mat->bind();

	/* Draw Shade */
	draw_shade();

	/* Draw Texture */
	draw_textures( false );

}

/******************************************************
*
* Draw Reflected Geometry Texture.
*
*******************************************************/
void Renderer::draw_textures_refl()
{
	/* Check if material exist */
	Material* mat = matMgrPtr->getMaterial( materialID );
	if ( !mat )
		return;

	/* Check if diffuse light is vaild */
	if ( mat->diff[3] <= 0.99 )
		return;

	/* Bind Material */
	mat->bind();

	/* Draw Shade Reflection */
	draw_shade_refl();

	/* Draw Texture Reflection */
	draw_textures( true );
}

/******************************************************
*
* Draw Geometry Texture.
*
*******************************************************/
void Renderer::draw_textures( bool reflFlag )
{
	glPushMatrix();
	if ( reflFlag )
		glMultMatrixf((GLfloat*)model_mat);
	else
		glMultMatrixf((GLfloat*)reflect_mat);

	/* Enable Lighting, Blend, and Depth Test */
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glDepthMask( GL_FALSE );
	glDepthFunc( GL_EQUAL );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0 );

	/* Apply Texture */
	for ( int i = 0; i < (int)appTexVec.size(); i++ )
	{
		Material* mat = matMgrPtr->getMaterial( materialID );
		Material wmat = matMgrPtr->getWhiteMaterial( (float)appTexVec[i].bright, mat->shine );
		wmat.diff[3] = (float)appTexVec[i].alpha;
		wmat.bind();

		glBindTexture( GL_TEXTURE_2D, appTexVec[i].texID );

		if ( appTexVec[i].repeatFlag )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}
		else
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		}

		for ( int j = 0; j < (int)surfVec.size(); j++)
		{
			if ( surfVec[j]->get_draw_flag() )
			{
				if ( appTexVec[i].surfID == j || appTexVec[i].allSurfFlag )
				{
					if ( reflFlag )
						surfVec[j]->draw_refl_texture( appTexVec[i], sym_code );
					else
						surfVec[j]->draw_texture( appTexVec[i] );
				}
			}
		}
	}

	/* Restore To Previous Setting */
	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LESS );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );

	glPopMatrix();
}

/******************************************************
*
* Draw Highlighting Box Around Xformed Geom.
*
*******************************************************/
void Renderer::draw_highlight_boxes()
{
	//==== Draw Current Comp Box =====//
	if ( redFlag == 1 )
	{
		glLineWidth( 2 );
      glColor3f( 1.0, 0, 0 );
      draw_bbox(); 
	}

	//==== Draw Temp Highlight Box =====//
	if ( yellowFlag == 1)
   {
		glLineWidth( 2 );
      glColor3f( 1.0, 1.0, 0 );
      draw_bbox(); 
	}
}

/******************************************************
*
* Draw Bounding Box Around Xformed Geom.
*
*******************************************************/
void Renderer::draw_bbox()
{
	double temp[3];

	temp[0] = bnd_box_xform.get_min(0);
	temp[1] = bnd_box_xform.get_min(1);
	temp[2] = bnd_box_xform.get_min(2);

	glBegin( GL_LINE_STRIP );
		glVertex3dv(temp);
		temp[0] = bnd_box_xform.get_max(0);
		glVertex3dv(temp);
		temp[1] = bnd_box_xform.get_max(1);
		glVertex3dv(temp);
		temp[2] = bnd_box_xform.get_max(2);
		glVertex3dv(temp);
		temp[0] = bnd_box_xform.get_min(0);
		glVertex3dv(temp);
		temp[2] = bnd_box_xform.get_min(2);
		glVertex3dv(temp);
		temp[1] = bnd_box_xform.get_min(1);
		glVertex3dv(temp);
		temp[2] = bnd_box_xform.get_max(2);
		glVertex3dv(temp);
		temp[0] = bnd_box_xform.get_max(0);
		glVertex3dv(temp);
		temp[2] = bnd_box_xform.get_min(2);
		glVertex3dv(temp);
	glEnd();

	glBegin( GL_LINE_STRIP );
		temp[2] = bnd_box_xform.get_max(2);
		glVertex3dv(temp);
		temp[1] = bnd_box_xform.get_max(1);
		glVertex3dv(temp);
	glEnd();

	glBegin( GL_LINE_STRIP );
		temp[2] = bnd_box_xform.get_min(2);
		glVertex3dv(temp);
		temp[0] = bnd_box_xform.get_min(0);
		glVertex3dv(temp);
	glEnd();

	glBegin( GL_LINE_STRIP );
		temp[2] = bnd_box_xform.get_max(2);
		glVertex3dv(temp);
		temp[1] = bnd_box_xform.get_min(1);
		glVertex3dv(temp);
	glEnd();
}

/******************************************************
*
* Draw Box.
*
*******************************************************/
void Renderer::draw_bbox( bbox & box )
{
	double temp[3];

	temp[0] = box.get_min(0);
	temp[1] = box.get_min(1);
	temp[2] = box.get_min(2);

	glBegin( GL_LINE_STRIP );
		glVertex3dv(temp);
		temp[0] = box.get_max(0);
		glVertex3dv(temp);
		temp[1] = box.get_max(1);
		glVertex3dv(temp);
		temp[2] = box.get_max(2);
		glVertex3dv(temp);
		temp[0] = box.get_min(0);
		glVertex3dv(temp);
		temp[2] = box.get_min(2);
		glVertex3dv(temp);
		temp[1] = box.get_min(1);
		glVertex3dv(temp);
		temp[2] = box.get_max(2);
		glVertex3dv(temp);
		temp[0] = box.get_max(0);
		glVertex3dv(temp);
		temp[2] = box.get_min(2);
		glVertex3dv(temp);
	glEnd();

	glBegin( GL_LINE_STRIP );
		temp[2] = box.get_max(2);
		glVertex3dv(temp);
		temp[1] = box.get_max(1);
		glVertex3dv(temp);
	glEnd();

	glBegin( GL_LINE_STRIP );
		temp[2] = box.get_min(2);
		glVertex3dv(temp);
		temp[0] = box.get_min(0);
		glVertex3dv(temp);
	glEnd();

	glBegin( GL_LINE_STRIP );
		temp[2] = box.get_max(2);
		glVertex3dv(temp);
		temp[1] = box.get_min(1);
		glVertex3dv(temp);
	glEnd();
}

/******************************************************
*
* Calculate And Store Model, View, Reflect Matrix.
*
*******************************************************/
void Renderer::storeModelMatrix(Matrix* viewMatrix, Matrix* modelMatrix, Matrix* reflMatrix, float* model_mat, float* reflect_mat)
{
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix->data());

	glPushMatrix();
	glMultMatrixf((GLfloat*)model_mat);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix->data());
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf((GLfloat*)reflect_mat);
	glGetFloatv(GL_MODELVIEW_MATRIX, reflMatrix->data());
	glPopMatrix();
}

/******************************************************
*
* Draw Geom Wire.
*
*******************************************************/
void Renderer::draw_wire()
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)model_mat ); 

	for ( int i = 0 ; i < (int)surfVec.size() ; i++ )
	{
		glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );	
		if ( surfVec[i]->get_draw_flag() )
		{
			surfVec[i]->draw_wire();
		}
	}
	glPopMatrix();
}

/******************************************************
*
* Draw Reflected Geom Wire.
*
*******************************************************/
void Renderer::draw_wire_refl()
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)reflect_mat); 

	for ( int i = 0 ; i < (int)surfVec.size() ; i++ )
	{
		glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );	
		if ( surfVec[i]->get_draw_flag() )
			surfVec[i]->draw_refl_wire(sym_code);
	}
	glPopMatrix();
}

/******************************************************
*
* Draw Shade.
*
*******************************************************/
void Renderer::draw_shade()
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)model_mat ); 

	for ( int i = 0 ; i < (int)surfVec.size() ; i++ )
	{
		if ( surfVec[i]->get_draw_flag() )
			surfVec[i]->draw_shaded();
	}

	glPopMatrix();
}

/******************************************************
*
* Draw Reflected Shade.
*
*******************************************************/
void Renderer::draw_shade_refl()
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)reflect_mat ); 

	for ( int i = 0; i < (int)surfVec.size(); i++ )
	{
		if ( surfVec[i]->get_draw_flag() )
			surfVec[i]->draw_refl_shaded( sym_code );
	}

	glPopMatrix();
}

/******************************************************
*
* Draw Hidden Geom.
*
*******************************************************/
void Renderer::draw_hidden()
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)model_mat); 

	for ( int i = 0 ; i < (int)surfVec.size() ; i++ )
	{
		if ( surfVec[i]->get_draw_flag() )
		{						
			glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );	
			surfVec[i]->draw_hidden();
		}
	}
	glPopMatrix();
}

/******************************************************
*
* Draw Reflected Hidden Geom.
*
*******************************************************/
void Renderer::draw_hidden_refl()
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)reflect_mat); 

	for ( int i = 0 ; i < (int)surfVec.size() ; i++ )
	{
		if ( surfVec[i]->get_draw_flag() )
		{
			glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );	
			surfVec[i]->draw_refl_hidden(sym_code);
		}
	}
	glPopMatrix();
}

/******************************************************
*
* Set Int Variables.
*
*******************************************************/
void Renderer::setValue( int value, int ID )
{
	switch ( ID )
	{
		case R_SYM_CODE:
			sym_code = value;
			break;

		case R_MATERIAL_ID:
			materialID = value;
			break;
		
		case R_RED_FLAG:
			redFlag = value;
			break;

		case R_YELLOW_FLAG:
			yellowFlag = value;
			break;

		case R_NOSHOW_FLAG:
			noshowFlag = value;
			break;

		case R_DISPLAY_FLAG:
			displayFlag = value;
			break;

		case R_FASTDRAW_FLAG:
			fastDrawFlag = value;
			break;
	}
}

/******************************************************
*
* Set Float Array Variables.
*
*******************************************************/
void Renderer::setValue( float* value, int ID )
{
	switch ( ID )
	{
	case R_MODEL_MATRIX:
		model_mat = value;
		break;

	case R_REFLECT_MATRIX:
		reflect_mat = value;
		break;
	}
}

/******************************************************
*
* Set Bounding Box Variables.
*
*******************************************************/
void Renderer::setValue( bbox value, int ID )
{
	switch ( ID )
	{
	case R_BOUNDING_BOX:
		bnd_box_xform = value;
		break;
	}
}

/******************************************************
*
* Set Cross Section Surface Renderer Variables.
*
*******************************************************/
void Renderer::setValue( vector<XSecSurfRenderer*> value, int ID )
{
	switch ( ID )
	{
	case R_XSECTION_RENDERS:
		surfVec = value;
	}
}

/******************************************************
*
* Set Texture Variables.
*
*******************************************************/
void Renderer::setValue( vector<AppliedTex> value, int ID )
{
	switch ( ID )
	{
	case R_TEXTURE_VEC:
		appTexVec = value;
	}
}

/******************************************************
*
* Set vec3d Variables.
*
*******************************************************/
void Renderer::setValue( vec3d value, int ID )
{
	switch ( ID )
	{
	case R_COLOR:
		color = value;
	}
}

/******************************************************
*
* Reset Variables To Default.
*
*******************************************************/
void Renderer::reset()
{
	/* Initialization */
	materialID = 0;
	sym_code = NO_SYM;
	redFlag = yellowFlag = noshowFlag = 0;
	displayFlag = GEOM_WIRE_FLAG;
	color = vec3d( 1, 1, 1 );  // default white

	static float identity_mat[16] =
		{ 1.0, 0.0, 0.0, 0.0,
		  0.0, 1.0, 0.0, 0.0,
		  0.0, 0.0, 1.0, 0.0,
		  0.0, 0.0, 0.0, 1.0 };

	/* Set defalut model_mat and reflect_mat to Identity Matrix */
	model_mat = identity_mat;
	reflect_mat = identity_mat;

	/* Set Some Reasonable Values For Bounding Box */
	bnd_box_xform.update( vec3d( 0.0001, 0.0001, 0.0001 ) );
	bnd_box_xform.update( vec3d( -0.0001, -0.0001, -0.0001 ) );
}

/******************************************************
*
* Blank Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
BlankGeomRenderer::BlankGeomRenderer() : Renderer()
{
}

BlankGeomRenderer::~BlankGeomRenderer()
{
}

/******************************************************
*
* Overloading from Renderer Class.
* Draw Blank Geometry.
*
*******************************************************/
void BlankGeomRenderer::draw()
{
	/* Check Noshow Flag */
	if ( noshowFlag ) return;

	/* Draw Blank Geometry */
	if ( displayFlag == GEOM_WIRE_FLAG && redFlag )
	{
		glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );

		glPushMatrix();
		glMultMatrixf( (GLfloat*)model_mat );

		glColor3ub( 255, 0, 255 );
		glPointSize( 10.0 );
		glBegin( GL_POINTS );
			glVertex3d( 0, 0, 0 );
		glEnd();

		glPopMatrix();
	}
}

/******************************************************
*
* Cabin Layout Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
CabinLayoutGeomRenderer::CabinLayoutGeomRenderer() : Renderer()
{
}

CabinLayoutGeomRenderer::~CabinLayoutGeomRenderer()
{
}


/******************************************************
*
* Duct Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
DuctGeomRenderer::DuctGeomRenderer() : Renderer()
{
}

DuctGeomRenderer::~DuctGeomRenderer()
{
}


/******************************************************
*
* Engine Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
EngineGeomRenderer::EngineGeomRenderer() : Renderer()
{
}

EngineGeomRenderer::~EngineGeomRenderer()
{
}

/******************************************************
*
* Ext Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
ExtGeomRenderer::ExtGeomRenderer() : Renderer()
{
}

ExtGeomRenderer::~ExtGeomRenderer()
{
}


/******************************************************
*
* Fuse Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
FuseGeomRenderer::FuseGeomRenderer() : Renderer()
{
}

FuseGeomRenderer::~FuseGeomRenderer()
{
}


/******************************************************
*
* Fuselage Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
FuselageGeomRenderer::FuselageGeomRenderer() : Renderer()
{
	if ( surfVec.size() > 0 )
	{
		/* FIXME: Hard coded primary surface, need implement a way
		   to identify surfaces*/
		primarySurf = surfVec[0];
	}
	else
	{
		primarySurf = NULL;
	}

	active_ctrl_pnt_id = -1;
}
FuselageGeomRenderer::~FuselageGeomRenderer()
{
}

/******************************************************
*
* Overloading From Renderer Class.
* Draw Fuselage Geom Wire.
*
*******************************************************/
void FuselageGeomRenderer::draw_wire()
{
	if ( !primarySurf )
		return;

	glPushAttrib( GL_CURRENT_BIT );
	glLineWidth( 1.0 );
	glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );

	int id_cnt = 0;
	int curr_edit_id;
	vector< int > edit_ids;
	for ( int i = 0; i < (int)surfVec.size(); i++ )
	{
		if ( i == curr_xsec_num )
			curr_edit_id = id_cnt;
		edit_ids.push_back( id_cnt );

		id_cnt += 1 + surfVec[i]->get_num_sect_interp();
	}

	int num_xsecs = primarySurf->get_num_xsecs();
	for ( int ixs = 0; ixs < num_xsecs; ixs++ )
	{
		if ( ixs == curr_edit_id )
		{
			glColor3ub( 255, 0, 0 );
			glLineWidth( 4.0 );
		}
		else if ( ContainsVal< int > ( edit_ids, ixs ) )
		{
			glColor3ub( (int)color.x(), (int)color.y(), (int)color.z() );
			glLineWidth( 2.0 );
		}
		else
		{
			glColor3ub( 100, 100, 100 );
			glLineWidth( 1.0 );
		}

		vector< vec3d > pnt_vec;
		primarySurf->get_xsec( ixs, pnt_vec );
		glBegin( GL_LINE_STRIP );
		for ( int j = 0; j < (int)pnt_vec.size(); j++ )
		{
			glVertex3dv( pnt_vec[j].data() );
		}
		glEnd();
	}

	glLineWidth( 1.0 );
	glColor3ub( 100, 100, 100 );
	int num_pnts = primarySurf->get_num_pnts();
	for ( int ipnt = 0; ipnt < num_pnts; ipnt++ )
	{
		vector< vec3d > pnt_vec;
		primarySurf->get_stringer( ipnt, pnt_vec );
		glBegin( GL_LINE_STRIP );
		for ( int j = 0; j < (int)pnt_vec.size(); j++ )
		{
			glVertex3dv( pnt_vec[j].data() );
		}
		glEnd();
	}
	glPopAttrib();
}

/******************************************************
*
* Draw Control Points.
*
*******************************************************/
void FuselageGeomRenderer::draw_control_points()
{
	//int i;
	//for ( i = 0; i < (int)cPntVec.size(); i++ )
	//{
	//	if ( active_ctrl_pnt_id == i )
	//	{
	//		if ( Fl::event_state( FL_SHIFT ) )
	//			glColor3ub( 255, 255, 0 );
	//		else
	//			glColor3ub( 255, 0, 0 );

	//		glBegin( GL_LINES );

	//		vec3d pnt = cPntVec[i]->pnt3d;
	//		glVertex3dv( pnt.data() );
	//		//if ( cPntVec[i]->pntID == ControlPnt::TOP )
	//			//pnt.offset_z(
	//	}
	//}
}

/******************************************************
*
* Set Control Points.
*
*******************************************************/
void FuselageGeomRenderer::setValue( vector< ControlPnt* > value, int ID )
{
	if ( ID = R_FUSELAGE_CTRL_PNTS )
	{
		cPntVec = value;
	}
}

/******************************************************
*
* Havoc Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
HavocGeomRenderer::HavocGeomRenderer() : Renderer()
{
}

HavocGeomRenderer::~HavocGeomRenderer()
{
}

/******************************************************
*
* HWB Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
HWBGeomRenderer::HWBGeomRenderer() : Renderer()
{
}

HWBGeomRenderer::~HWBGeomRenderer()
{
}

/******************************************************
*
* External Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
MeshGeomRenderer::MeshGeomRenderer() : Renderer()
{
}

MeshGeomRenderer::~MeshGeomRenderer()
{
}

//void MeshGeomRenderer::draw()
//{
//
//}
//
//void MeshGeomRenderer::drawAlpha()
//{
//
//}

/******************************************************
*
* MS Wing Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
MSWingGeomRenderer::MSWingGeomRenderer() : Renderer()
{
	highLightType = MSW_HIGHLIGHT_NONE;
	section_Box.update( vec3d( 0.0001, 0.0001, 0.0001 ) );
	section_Box.update( vec3d( -0.0001, -0.0001, -0.0001 ) ); 
}
MSWingGeomRenderer::~MSWingGeomRenderer()
{
}

/******************************************************
*
* Overloading from Renderer Class.
* Draw Multi Section Wing.
*
*******************************************************/
void MSWingGeomRenderer::draw()
{
	Renderer::draw();

	if ( displayFlag == GEOM_WIRE_FLAG )
	{
		if ( redFlag )
		{
			glLineWidth( 2 );
			glColor3f( 0.75, 0, 0 );
			if ( highLightType != MSW_HIGHLIGHT_NONE )
			{
				draw_sect_box();
			}
		}
	}
}

/******************************************************
*
* Draw Section Box.
*
*******************************************************/
void MSWingGeomRenderer::draw_sect_box()
{
	Renderer::draw_bbox( section_Box );
}

/******************************************************
*
* Overloading from Renderer Class.
* Set Int Value.
*
*******************************************************/
void MSWingGeomRenderer::setValue( int value, int ID )
{
	Renderer::setValue( value, ID );
	if ( ID == R_MSWING_HIGHLIGHT )
	{
		highLightType = value;
	}
}

/******************************************************
*
* Overloading from Renderer Class.
* Set bbox Value.
*
*******************************************************/
void MSWingGeomRenderer::setValue( bbox value, int ID )
{
	Renderer::setValue( value, ID );
	if ( ID == R_MSWING_SEC_BOX )
	{
		section_Box = value;
	}
}

/******************************************************
*
* Overloading from Renderer Class.
* Reset to Default.
*
*******************************************************/
void MSWingGeomRenderer::reset()
{
	Renderer::reset();
	highLightType = MSW_HIGHLIGHT_NONE;
	section_Box.update( vec3d( 0.0001, 0.0001, 0.0001 ) );
	section_Box.update( vec3d( -0.0001, -0.0001, -0.0001 ) ); 
}

/******************************************************
*
* Pod Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
PodGeomRenderer::PodGeomRenderer() : Renderer()
{
}

PodGeomRenderer::~PodGeomRenderer()
{
}


/******************************************************
*
* Prop Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
PropGeomRenderer::PropGeomRenderer() : Renderer()
{
}

PropGeomRenderer::~PropGeomRenderer()
{
}


/******************************************************
*
* Vor Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
VorGeomRenderer::VorGeomRenderer() : Renderer()
{
}

VorGeomRenderer::~VorGeomRenderer()
{
}


/******************************************************
*
* Wing Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
WingGeomRenderer::WingGeomRenderer() : Renderer()
{
}

WingGeomRenderer::~WingGeomRenderer()
{
}

/******************************************************
*
* Overloading from Renderer Class.
* Draw Wing Geometry.
*
*******************************************************/
void WingGeomRenderer::draw()
{
	int i;

	/* FastDraw Flag */
	for ( i = 0; i < (int)surfVec.size(); i++ )
	{
		if ( fastDrawFlag )
		{
			surfVec[i]->fast_draw_on();
		}
		else
		{
			surfVec[i]->fast_draw_off();
		}
	}

	/* Draw Highlighting Boxes */
	draw_highlight_boxes();

	/* Check Noshow Flag */
	if ( noshowFlag ) return;	

	if ( displayFlag == GEOM_WIRE_FLAG )
	{
		/* Draw Geom Wire */
		draw_wire();
		
		/* Draw Reflected Geom Wire */
		draw_wire_refl();
	}
	else if ( displayFlag == GEOM_SHADE_FLAG )
	{
		/* Draw Geom */
		draw_shade();

		/* Reflected Geom */
		draw_shade_refl();
	}
	else if ( displayFlag == GEOM_HIDDEN_FLAG )
	{
		/* Draw Geom */
		draw_hidden();

		/* Reflected Geom */
		draw_hidden_refl();
	}
}

/******************************************************
*
* Overloading from Renderer Class.
* Draw Wing Geom With Transparent Effect.
*
*******************************************************/
void WingGeomRenderer::drawAlpha()
{
	int i;

	/* Check Noshow Flag */
	if ( noshowFlag ) return;	

	/* Update FastDraw Flag */
	for ( i = 0; i < (int)surfVec.size(); i++ )
	{
		if ( fastDrawFlag )
		{
			surfVec[i]->fast_draw_on();
		}
		else
		{
			surfVec[i]->fast_draw_off();
		}
	}

	/* Draw Shaded Wing Geom */
	if ( displayFlag == GEOM_SHADE_FLAG )
	{
		draw_shade();
		draw_shade_refl();
	}
}

/******************************************************
*
* Overloading from Renderer Class.
* Draw Shaded Wing Geom.
*
*******************************************************/
void WingGeomRenderer::draw_shade()
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)model_mat ); 
	Material* mat = matMgrPtr->getMaterial( materialID );
	if ( mat )
	{
		mat->bind();
		if  ( mat->diff[3] > 0.99 )
		{ 
			/*FIXME: mwing surface is hard coded
			as first item in the list ( see WingGeom.cpp: getXSectionSurfVec() ).
			Need implement something to identify cross 
			section surfaces in vector. */

			surfVec[0]->draw_shaded(); // mswing_surf
		}
	}
	glPopMatrix();
}

/******************************************************
*
* Overloading from Renderer Class.
* Draw Reflected Shaded Wing Geom.
*
*******************************************************/
void WingGeomRenderer::draw_shade_refl()
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)reflect_mat ); 
	Material* mat = matMgrPtr->getMaterial( materialID );
	if ( mat )
	{
		mat->bind();
		if  ( mat->diff[3] > 0.99 )
		{ 
			/*FIXME: mwing surface is hard coded
			as first item in the list( see WingGeom.cpp: getXSectionSurfVec() ).
			Need implement something to identify cross 
			section surfaces in vector. */

			surfVec[0]->draw_shaded(); // mswing_surf
		}
	}
	glPopMatrix();
}

/******************************************************
*
* X Section Geometry Renderer Class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
XSecGeomRenderer::XSecGeomRenderer() : Renderer()
{
}

XSecGeomRenderer::~XSecGeomRenderer()
{
}