//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// textureMgrScreen.cpp: implementation of the geomScreen class.
//
//////////////////////////////////////////////////////////////////////
#include "materialMgr.h"
#include "textureMgrScreen.h"
#include <FL/Fl_File_Chooser.H>

TexWinDraw::TexWinDraw()
{
	m_TexPtr = NULL;

	rp_texture.mode.texture2DMode.enabled = true;

	rp_texture.mode.texture2DMode.texParameteri.pname.clear();
	rp_texture.mode.texture2DMode.texParameteri.param.clear();

	rp_texture.mode.texture2DMode.texParameteri.pname.push_back( R_TEXTURE_WRAP_S );
	rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );

	rp_texture.mode.texture2DMode.texParameteri.pname.push_back( R_TEXTURE_WRAP_T );
	rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );
	
	rp_texture.mode.blendMode.enabled = true;
	rp_texture.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_texture.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;

	rp_texture.mode.alphaTestMode.enabled = true;
	rp_texture.mode.alphaTestMode.alphafunc.func = R_GREATER;
	rp_texture.mode.alphaTestMode.alphafunc.ref = 0;
}

TexWinDraw::~TexWinDraw()
{

}

void TexWinDraw::SetTexture( AppliedTex * tex )
{
	m_TexPtr = tex;
}

void TexWinDraw::draw()
{
	int i;
	vector<double> data, colors, texcoords;

	//==== Draw Grid ====//
	float gridSize = 0.1f;

	renderer->setLineWidth( 1.0 );
	renderer->setColor3d( 0.8f, 0.8f, 0.8f );

	for ( i = 0 ; i < 41 ; i++ )
	{
		if ( i == 20 )
		{
			for ( int vIter = 0; vIter < 4; vIter++ )
			{
				for ( int cIter = 0; cIter < 3; cIter++ )
				{
					colors.push_back( 0.8 );
				}
			}
		}
		else
		{
			for ( int vIter = 0; vIter < 4; vIter++ )
			{
				for ( int cIter = 0; cIter < 3; cIter++ )
				{
					colors.push_back( 0.9 );
				}
			}
		}

		data.push_back( gridSize*(float)i - 20.0f*gridSize );
		data.push_back( -20.0f*gridSize );

		data.push_back( gridSize*(float)i - 20.0f*gridSize );
		data.push_back( 20.0f*gridSize );

		data.push_back( -20.0f*gridSize );
		data.push_back( gridSize*(float)i - 20.0f*gridSize );

		data.push_back( 20.0f*gridSize );
		data.push_back( gridSize*(float)i - 20.0f*gridSize );
	}
	renderer->draw( R_LINES, 3, colors, 2, data );
	data.clear();

	if ( m_TexPtr )
	{  
		renderer->setColor4d( 1.0, 1.0, 1.0, m_TexPtr->alpha );

		rp_texture.mode.texture2DMode.bindTexture.texture = m_TexPtr->texID;

		rp_texture.mode.texture2DMode.texParameteri.param.clear();
		if ( m_TexPtr->repeatFlag )
		{
			rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_REPEAT );
			rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_REPEAT );
		}
		else
		{
			rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );
			rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );
		}

		texcoords.push_back( 0.0 );
		texcoords.push_back( 0.0 );

		texcoords.push_back( 1.0 );
		texcoords.push_back( 0.0 );

		texcoords.push_back( 1.0 );
		texcoords.push_back( 1.0 );

		texcoords.push_back( 0.0 );
		texcoords.push_back( 1.0 );

		data.push_back( -0.5 );
		data.push_back( -0.5 );

		data.push_back( 0.5);
		data.push_back( -0.5 );

		data.push_back( 0.5);
		data.push_back( 0.5 );

		data.push_back( -0.5);
		data.push_back( 0.5 );

		renderer->draw( R_QUADS, rp_texture, 2, data, 2, texcoords );
	}
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TextureMgrScreen::TextureMgrScreen(ScreenMgr* mgr, Aircraft* airPtr)
{
	screenMgrPtr = mgr;
	aircraftPtr = airPtr;
	m_currGeomID = 0;

	TextureMgrUI* ui = textureMgrUI = new TextureMgrUI();
	textureMgrUI->UIWindow->position( 775, 30 );

	m_UPosSlider = new SliderInputCombo( ui->uPosSlider, ui->uPosInput );
	m_UPosSlider->SetCallback( staticScreenCB, this );
	m_UPosSlider->SetLimits( 0.0, 1.0 );
	m_UPosSlider->SetRange( 1.0 );
	m_UPosSlider->UpdateGui();
	m_WPosSlider = new SliderInputCombo( ui->wPosSlider, ui->wPosInput );
	m_WPosSlider->SetCallback( staticScreenCB, this );
	m_WPosSlider->SetLimits( 0.0, 1.0 );
	m_WPosSlider->SetRange( 1.0 );
	m_WPosSlider->UpdateGui();

	m_UScaleSlider = new SliderInputCombo( ui->uScaleSlider, ui->uScaleInput );
	m_UScaleSlider->SetCallback( staticScreenCB, this );
	m_UScaleSlider->SetLimits( 0.01, 1.0 );
	m_UScaleSlider->SetRange( 1.0 );
	m_UScaleSlider->UpdateGui();
	m_WScaleSlider = new SliderInputCombo( ui->wScaleSlider, ui->wScaleInput );
	m_WScaleSlider->SetCallback( staticScreenCB, this );
	m_WScaleSlider->SetLimits( 0.01, 1.0 );
	m_WScaleSlider->SetRange( 1.0 );
	m_WScaleSlider->UpdateGui();

	m_BrightSlider = new SliderInputCombo( ui->brightSlider, ui->brightInput );
	m_BrightSlider->SetCallback( staticScreenCB, this );
	m_BrightSlider->SetLimits( 0.01, 1.0 );
	m_BrightSlider->SetRange( 1.0 );
	m_BrightSlider->UpdateGui();
	m_AlphaSlider = new SliderInputCombo( ui->alphaSlider, ui->alphaInput );
	m_AlphaSlider->SetCallback( staticScreenCB, this );
	m_AlphaSlider->SetLimits( 0.01, 1.0 );
	m_AlphaSlider->SetRange( 1.0 );
	m_AlphaSlider->UpdateGui();

	ui->repeatTextureButton->callback( staticScreenCB, this );
	ui->flipUButton->callback( staticScreenCB, this );
	ui->flipWButton->callback( staticScreenCB, this );
	ui->reflFlipUButton->callback( staticScreenCB, this );
	ui->reflFlipWButton->callback( staticScreenCB, this );
	ui->textureNameInput->callback( staticScreenCB, this );

	ui->addTextureButton->callback( staticScreenCB, this );
	ui->delTextureButton->callback( staticScreenCB, this );
	ui->textureSurfChoice->callback( staticScreenCB, this );

	ui->textureChoice->callback( staticScreenCB, this );
	ui->compChoice->callback( staticScreenCB, this );

	//==== Add Gl Window to Main Window ====//
	Fl_Widget* w = ui->texGLGroup;
	ui->texGLGroup->begin();
	m_TexGLWin = new XSecGlWindow(w->x(), w->y(), w->w(), w->h()); 
	ui->texGLGroup->end();

	m_TexGLWin->setDrawBase( &m_TexWinDraw );


}

TextureMgrScreen::~TextureMgrScreen()
{

}

void TextureMgrScreen::update()
{
	int i;
	char str[256];

	//==== Load Geom Choice ====//
	vector< Geom* > geomVec = aircraftPtr->getGeomVec();	
	textureMgrUI->compChoice->clear();
	for ( i = 0 ; i < (int)geomVec.size() ; i++ )
	{
		textureMgrUI->compChoice->add( geomVec[i]->getName() );
	}

	Geom* currGeom = NULL;
	if ( m_currGeomID >= 0 && m_currGeomID < (int)geomVec.size() )
	{
		currGeom = geomVec[m_currGeomID];
		textureMgrUI->compChoice->value( m_currGeomID );
	}
	//==== Clear Choice and Input ====//
	textureMgrUI->textureChoice->clear();
	textureMgrUI->textureChoice->add( " " );
	textureMgrUI->textureChoice->value(0);
	textureMgrUI->textureNameInput->value( "" );
	deactivate();

	if ( currGeom )
	{
		if ( currGeom->getNumTex() )
			textureMgrUI->textureChoice->clear();
		for (  i = 0 ; i < currGeom->getNumTex() ; i++ )
		{
			AppliedTex* tex = currGeom->getTex( i );
			if ( tex )
			{
				sprintf( str, "%d", i );
				textureMgrUI->textureChoice->add( str );	// Hack to get dupicate name in list
			}
		}
		for (  i = 0 ; i < currGeom->getNumTex() ; i++ )
		{
			AppliedTex* tex = currGeom->getTex( i );
			if ( tex )
				textureMgrUI->textureChoice->replace( i, tex->nameStr.get_char_star() );
		}

		int currTexID = currGeom->getCurrentTexID();
		if ( currTexID >= 0 )
			textureMgrUI->textureChoice->value( currTexID );

		AppliedTex* currTex = currGeom->getTex( currTexID );
		m_TexWinDraw.SetTexture( currTex );
		m_TexGLWin->redraw();

		if ( currTex )
		{
			activate();
			m_UPosSlider->SetVal( currTex->u );
			m_WPosSlider->SetVal( currTex->w );
			m_UScaleSlider->SetVal( currTex->scaleu );
			m_WScaleSlider->SetVal( currTex->scalew );
			m_BrightSlider->SetVal( currTex->bright );
			m_AlphaSlider->SetVal( currTex->alpha );

			m_UPosSlider->UpdateGui( );
			m_WPosSlider->UpdateGui( );
			m_UScaleSlider->UpdateGui( );
			m_WScaleSlider->UpdateGui( );
			m_BrightSlider->UpdateGui( );
			m_AlphaSlider->UpdateGui( );

			textureMgrUI->repeatTextureButton->value( currTex->repeatFlag );
			textureMgrUI->flipUButton->value( currTex->flipUFlag );
			textureMgrUI->flipWButton->value( currTex->flipWFlag );
			textureMgrUI->reflFlipUButton->value( currTex->reflFlipUFlag );
			textureMgrUI->reflFlipWButton->value( currTex->reflFlipWFlag );

			textureMgrUI->textureNameInput->value( currTex->nameStr );

			textureMgrUI->textureSurfChoice->clear();
			textureMgrUI->textureSurfChoice->add("All");
			for ( i = 0 ; i < currGeom->getNumSurf() ; i++ )
			{
				char str[256];
				sprintf( str, "%d", i+1 );
				textureMgrUI->textureSurfChoice->add(str);
			}
			if ( currTex->allSurfFlag )
				textureMgrUI->textureSurfChoice->value( 0 );
			else
				textureMgrUI->textureSurfChoice->value( currTex->surfID+1 );

		}
	}
}

void TextureMgrScreen::activate()
{
	m_UPosSlider->Activate();
	m_WPosSlider->Activate();
	m_UScaleSlider->Activate();
	m_WScaleSlider->Activate();
	m_BrightSlider->Activate();
	m_AlphaSlider->Activate();
	textureMgrUI->textureSurfChoice->activate();
}
void TextureMgrScreen::deactivate()
{
	m_UPosSlider->Deactivate();
	m_WPosSlider->Deactivate();
	m_UScaleSlider->Deactivate();
	m_WScaleSlider->Deactivate();
	m_BrightSlider->Deactivate();
	m_AlphaSlider->Deactivate();
	textureMgrUI->textureSurfChoice->deactivate();
}


void TextureMgrScreen::show()
{
	update();
	textureMgrUI->UIWindow->show();
}


void TextureMgrScreen::screenCB( Fl_Widget* w )
{
	vector< Geom* > geomVec = aircraftPtr->getGeomVec();	

	Geom* currGeom = NULL;
	if ( m_currGeomID >= 0 && m_currGeomID < (int)geomVec.size() )
		currGeom = geomVec[m_currGeomID];

	if ( !currGeom )
	{
		update();
		return;
	}

	AppliedTex* currTex = currGeom->getTex( currGeom->getCurrentTexID() );

	if ( m_UPosSlider->GuiChanged( w ) && currTex )
	{
		currTex->u = m_UPosSlider->GetVal();
	}
	else if ( m_WPosSlider->GuiChanged( w ) && currTex )
	{
		currTex->w = m_WPosSlider->GetVal();
	}
	else if ( m_UScaleSlider->GuiChanged( w ) && currTex )
	{
		currTex->scaleu = m_UScaleSlider->GetVal();
	}
	else if ( m_WScaleSlider->GuiChanged( w ) && currTex )
	{
		currTex->scalew = m_WScaleSlider->GetVal();
	}
	else if ( m_BrightSlider->GuiChanged( w ) && currTex )
	{
		currTex->bright = m_BrightSlider->GetVal();
	}
	else if ( m_AlphaSlider->GuiChanged( w ) && currTex )
	{
		currTex->alpha = m_AlphaSlider->GetVal();
		m_TexGLWin->redraw();
	}
	else if ( w == textureMgrUI->repeatTextureButton && currTex )
	{
		currTex->repeatFlag = !!textureMgrUI->repeatTextureButton->value();
	}
	else if ( w == textureMgrUI->flipUButton && currTex )
	{
		currTex->flipUFlag = !!textureMgrUI->flipUButton->value();
	}
	else if ( w == textureMgrUI->flipWButton && currTex )
	{
		currTex->flipWFlag = !!textureMgrUI->flipWButton->value();
	}
	else if ( w == textureMgrUI->reflFlipUButton && currTex )
	{
		currTex->reflFlipUFlag = !!textureMgrUI->reflFlipUButton->value();
	}
	else if ( w == textureMgrUI->reflFlipWButton && currTex )
	{
		currTex->reflFlipWFlag = !!textureMgrUI->reflFlipWButton->value();
	}
	else if ( w == textureMgrUI->textureNameInput && currTex )
	{
		currTex->nameStr = textureMgrUI->textureNameInput->value();
		update();
	}
	else if ( w == textureMgrUI->textureSurfChoice && currTex )
	{
		currTex->allSurfFlag = false;
		if ( textureMgrUI->textureSurfChoice->value() == 0 )
			currTex->allSurfFlag = true;
		else
			currTex->surfID = textureMgrUI->textureSurfChoice->value()-1;
	}
	else if ( w == textureMgrUI->addTextureButton )
	{
		char *newfile = fl_file_chooser("Read Texture?", "*.{tga,jpg}", "");
		if ( newfile != NULL )
		{
			currGeom->addTex(newfile);
			update();
		}
	}
	else if ( w == textureMgrUI->delTextureButton )
	{
		currGeom->delCurrentTex();
		update();
	}
	else if ( w == textureMgrUI->compChoice )
	{
		m_currGeomID = textureMgrUI->compChoice->value();
		update();
	}
	else if ( w == textureMgrUI->textureChoice )
	{
		currGeom->setCurrentTexID( textureMgrUI->textureChoice->value() );
		update();
	}


	currGeom->triggerDraw();
}

