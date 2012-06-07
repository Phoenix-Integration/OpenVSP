#include "feaMeshRenderer.h"
#include "common.h"

/******************************************************
*
* FEA Mesh Renderer.
*
* This class wraps fea_mesh_renderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
feaRenderer::feaRenderer( FeaMeshMgr* feaMesh )
{
	feaPtr = feaMesh;
}
feaRenderer::~feaRenderer()
{
}

/******************************************************
*
* Draw FEA Mesh.
*
*******************************************************/
void feaRenderer::draw()
{
	/* Draw Mesh */
	if ( feaPtr->GetDrawMeshFlag() )
	{
		/* Draw Ribs Spars */
		drawRibSparElements();

		/* Draw Skin*/
		drawSkin();

		/* Draw Point Mass and Attachment Points */
		if ( feaPtr->GetCurrEditType() == POINT_MASS_EDIT )
		{
			drawPointMass();

			if ( feaPtr->GetDrawAttachPointsFlag() )
			{
				drawAttachmentPoints();
			}
		}
	}
	/* Draw Pre-Mesh */
	else
	{
		/* Draw Rib Spar Pre-Mesh Representations. */
		drawRibs();
		drawSpars();

		/* Draw Wing Section Pre-Mesh Representations. */
		drawWingSections();
	}
}

/******************************************************
*
* Draw Rib Pre-Mesh Representations ( No Mesh ).
*
*******************************************************/
void feaRenderer::drawRibs()
{
	FeaRib* curr_rib = feaPtr->GetCurrRib();

	for ( int i = 0; i < feaPtr->GetWingSectionSize(); i++ )
	{
		for ( int j = 0; j < ( int )feaPtr->GetWingSection( i )->m_RibVec.size(); j++ )
		{
			FeaRib* rib = feaPtr->GetWingSection( i )->m_RibVec[j];

			if ( feaPtr->GetCurrEditType() == RIB_EDIT && rib == curr_rib )
			{
				ribRenderer renderer = ribRenderer( rib );
				renderer.init();
				renderer.highlightOn();
				renderer.draw();
			}
			else
			{
				ribRenderer renderer = ribRenderer( rib );
				renderer.init();
				renderer.highlightOff();
				renderer.draw();
			}
		}
	}
}

/******************************************************
*
* Draw Spars Pre-Mesh Representations ( No Mesh ).
*
*******************************************************/
void feaRenderer::drawSpars()
{
	FeaSpar* curr_spar = feaPtr->GetCurrSpar();

	for ( int i = 0; i < ( int )feaPtr->GetWingSectionSize(); i++ )
	{
		for ( int j = 0; j < ( int )feaPtr->GetWingSection( i )->m_SparVec.size(); j++ )
		{
			FeaSpar* spar = feaPtr->GetWingSection( i )->m_SparVec[j];
			if ( feaPtr->GetCurrEditType() == SPAR_EDIT && spar == curr_spar )
			{
				sparRenderer renderer = sparRenderer( spar );
				renderer.init();
				renderer.highlightOn();
				renderer.draw();
			}
			else
			{
				sparRenderer renderer = sparRenderer( spar );
				renderer.init();
				renderer.highlightOff();
				renderer.draw();
			}
		}
	}
}

/******************************************************
*
* Draw Wing Section Pre-Mesh Representations.
*
*******************************************************/
void feaRenderer::drawWingSections()
{
	for ( int i = 0; i < ( int )feaPtr->GetWingSectionSize(); i++ )
	{
		if ( i == feaPtr->GetCurrSectID() )
		{
			/* Highlight ON */

			/* Draw Upper Skin */
			if ( feaPtr->GetCurrEditType() == UP_SKIN_EDIT )
			{
				skinRenderer renderer = skinRenderer( &feaPtr->GetWingSection(i)->m_UpperSkin );
				renderer.init();
				renderer.highlightOn();
				renderer.draw();
			}
			/* Draw Lower Skin */
			else if ( feaPtr->GetCurrEditType() == LOW_SKIN_EDIT )
			{
				skinRenderer renderer = skinRenderer( &feaPtr->GetWingSection(i)->m_LowerSkin );
				renderer.init();
				renderer.highlightOn();
				renderer.draw();
			}

			/* Draw Wing Section */
			wingSectionRenderer renderer = wingSectionRenderer( feaPtr->GetWingSection(i) );
			renderer.init();
			renderer.highlightOn();
			renderer.draw();
		}
		else
		{
			/* Highlight OFF */

			/* Draw Upper Skin */
			if ( feaPtr->GetCurrEditType() == UP_SKIN_EDIT )
			{
				skinRenderer renderer = skinRenderer( &feaPtr->GetWingSection(i)->m_UpperSkin );
				renderer.init();
				renderer.highlightOff();
				renderer.draw();
			}
			/* Draw Lower Skin */
			else if ( feaPtr->GetCurrEditType() == LOW_SKIN_EDIT )
			{
				skinRenderer renderer = skinRenderer( &feaPtr->GetWingSection(i)->m_LowerSkin );
				renderer.init();
				renderer.highlightOff();
				renderer.draw();
			}

			/* Draw Wing Section */
			wingSectionRenderer renderer = wingSectionRenderer( feaPtr->GetWingSection(i) );
			renderer.init();
			renderer.highlightOff();
			renderer.draw();
		}
	}
}

/******************************************************
*
* Draw Ribs and Spars with Mesh and Wire Frame.
*
*******************************************************/
void feaRenderer::drawRibSparElements()
{
	/* Initialize Renderer */
	feaElementRenderer renderer = feaElementRenderer();
	vector< vec3d > corners;

	/* Draw Ribs and Spars */
	for ( int i = 0; i < ( int )feaPtr->GetSliceVec().size(); i++ )
	{
		for ( int j = 0; j < ( int )feaPtr->GetSliceVec()[i]->m_Elements.size(); j++ )
		{
			FeaElement* fe = feaPtr->GetSliceVec()[i]->m_Elements[j];

			/* Set Rendering Data */
			corners.clear();
			for ( int k = 0; k < fe->m_Corners.size(); k++ )
			{
				corners.push_back( fe->m_Corners[k]->m_Pnt );
			}
			renderer.setValue( corners, R_FEA_ELEMENT_CORNER_PNTS );

			/* Draw Skin Mesh */
			renderer.set_Color( 0, 0, 250, 255 );
			renderer.draw();

			/* Draw Skin Wire Frame */
			renderer.set_Line_Width( 2.0 );
			renderer.set_Color( 0, 0, 0, 255 );
			renderer.drawWire();
		}
	}
}

/******************************************************
*
* Draw Skin.
*
*******************************************************/
void feaRenderer::drawSkin()
{
	/* Initialize Renderer */
	feaElementRenderer renderer = feaElementRenderer();
	vector< vec3d > corners;

	/* Enable Cull Back Faces for Trans */
	renderer.enableCullFace( BACK );

	/* Draw Skin */
	for ( int i = 0; i < ( int )feaPtr->GetSkinVec().size(); i++ )
	{
		for ( int j = 0; j < ( int )feaPtr->GetSkinVec()[i]->m_Elements.size(); j++ )
		{
			FeaElement* fe = feaPtr->GetSkinVec()[i]->m_Elements[j];

			/* Set Rendering Data */
			corners.clear();
			for ( int k = 0; k < fe->m_Corners.size(); k++ )
			{
				corners.push_back( fe->m_Corners[k]->m_Pnt );
			}
			renderer.setValue( corners, R_FEA_ELEMENT_CORNER_PNTS );

			/* Draw Skin Mesh */
			renderer.set_Color( 150, 150, 150, 50 );
			renderer.draw();

			/* Draw Skin Wire Frame */
			renderer.set_Line_Width( 2.0 );
			renderer.set_Color( 0, 0, 0, 100 );
			renderer.drawWire();
		}
	}
	renderer.disableCullFace();
}

/******************************************************
*
* Draw Potential Point Mass.
*
*******************************************************/
void feaRenderer::drawPointMass()
{
	for ( int i = 0; i < ( int )feaPtr->GetPointMassVec().size(); i++ )
	{
		pointMassRenderer renderer = pointMassRenderer( feaPtr->GetPointMassVec()[i] );
		renderer.init();

		if ( i == feaPtr->GetCurrPointMassID() )
		{
			renderer.highlightOn();
			renderer.draw();
		}
		else
		{
			renderer.highlightOff();
			renderer.draw();
		}
	}
}

/******************************************************
*
* Draw Attachment Points.
*
*******************************************************/
void feaRenderer::drawAttachmentPoints()
{
	attachmentPointsRenderer renderer = attachmentPointsRenderer( feaPtr );
	renderer.init();
	renderer.draw();
}

/******************************************************
*
* FEA Slice Renderer.
*
* This class wraps feaSliceRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Desctructor .
*
*******************************************************/
sliceRenderer::sliceRenderer( FeaSlice* feaSlice )
{
	slicePtr = feaSlice;
	renderPtr = NULL;
}
sliceRenderer::~sliceRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void sliceRenderer::init()
{
	/* Just In Case init() is Called Multiple Times */
	if ( renderPtr )
		delete renderPtr;

	/* Initialize Renderer */
	renderPtr = new feaSliceRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void sliceRenderer::load()
{
	/* Make sure to call init() first */
	if ( !renderPtr )
		return;

	/* Set rendering data */
	renderPtr->setValue( slicePtr->getUpperPnts(), R_FEA_SLICE_UPPER_PNTS );
	renderPtr->setValue( slicePtr->getLowerPnts(), R_FEA_SLICE_LOWER_PNTS );
}

/******************************************************
*
* Draw FEA Slice.
*
*******************************************************/
void sliceRenderer::draw()
{
	if ( !renderPtr )
		return;

	load();
	renderPtr->draw();
	clean();
}

/******************************************************
*
* Free Resources / Reset variables. 
* This Function is Called Internally.
*
*******************************************************/
void sliceRenderer::clean()
{
	if ( !renderPtr )
		return;

	renderPtr->reset();
}

/******************************************************
*
* Turn on highlight.
*
*******************************************************/
void sliceRenderer::highlightOn()
{
	if ( !renderPtr )
		return;

	/* Set highlight flag to true */
	renderPtr->setValue( true, R_FEA_SLICE_HIGHLIGHT_FLAG );
}

/******************************************************
*
* Turn off highlight.
*
*******************************************************/
void sliceRenderer::highlightOff()
{
	if ( !renderPtr )
		return;

	/* Set highlight flag to false */
	renderPtr->setValue( false, R_FEA_SLICE_HIGHLIGHT_FLAG );
}

/******************************************************
*
* FEA Rib Renderer.
*
* This class wraps feaRibRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
ribRenderer::ribRenderer( FeaSlice* slice ) : sliceRenderer( slice )
{
}
ribRenderer::~ribRenderer()
{
}

/******************************************************
*
* Overloading init() from sliceRenderer.
* Must Call Before Any Draw Functions.
*
*******************************************************/
void ribRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaRibRenderer();
}

/******************************************************
*
* Overloading load() from sliceRenderer. 
* This Function is Called Internally.
*
*******************************************************/
void ribRenderer::load()
{
	if ( !renderPtr )
		return;

	/* Convert upper endpoint array to EndPoints */
	EndPoints endpoints;
	endpoints.start = slicePtr->getUpperEndPnts()[0];
	endpoints.end = slicePtr->getUpperEndPnts()[1];

	/* Get plane data from Surf */
	Surf* surf = slicePtr->GetSurf();
	vector< vec3d > planeData;
	planeData.push_back( surf->CompPnt01( 0, 0 ) );
	planeData.push_back( surf->CompPnt01( 1, 0 ) );
	planeData.push_back( surf->CompPnt01( 1, 1 ) );
	planeData.push_back( surf->CompPnt01( 0, 1 ) );

	/* Set values */
	renderPtr->setValue( planeData, R_FEA_SLICE_PLANEDATA );
	renderPtr->setValue( endpoints, R_FEA_SLICE_UPPER_END_PNTS );
}

/******************************************************
*
* FEA Spar Renderer.
*
* This class wraps feaSparRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
sparRenderer::sparRenderer( FeaSlice* slice ) : sliceRenderer( slice )
{
}
sparRenderer::~sparRenderer()
{
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void sparRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaSparRenderer();
}

/******************************************************
*
* Overloading load() from sliceRenderer. 
* This Function is Called Internally.
*
*******************************************************/
void sparRenderer::load()
{
	if ( !renderPtr )
		return;
	
	/* Get plane data from Surf */
	Surf* surf = slicePtr->GetSurf();
	vector< vec3d > planeData;
	planeData.push_back( surf->CompPnt01( 0, 0 ) );
	planeData.push_back( surf->CompPnt01( 1, 0 ) );
	planeData.push_back( surf->CompPnt01( 1, 1 ) );
	planeData.push_back( surf->CompPnt01( 0, 1 ) );

	/* Set rendering data */
	renderPtr->setValue( planeData, R_FEA_SLICE_PLANEDATA );
	renderPtr->setValue( slicePtr->getUpperChainPnts(), R_FEA_SLICE_UPPER_CHAIN_PNTS );
	renderPtr->setValue( slicePtr->getLowerChainPnts(), R_FEA_SLICE_LOWER_CHAIN_PNTS );
}

/******************************************************
*
* FEA Wing Section Renderer.
*
* This class wraps feaWingRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
wingSectionRenderer::wingSectionRenderer( WingSection* wingSection )
{
	wingSectPtr = wingSection;
	renderPtr = NULL;
}
wingSectionRenderer::~wingSectionRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void wingSectionRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaWingRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void wingSectionRenderer::load()
{
	if ( !renderPtr )
		return;

	/* Convert corner array to vector */
	vector< vec3d > corners;
	corners.push_back( wingSectPtr->GetCornerPnts()[UW00] );
	corners.push_back( wingSectPtr->GetCornerPnts()[UW10] );
	corners.push_back( wingSectPtr->GetCornerPnts()[UW11] );
	corners.push_back( wingSectPtr->GetCornerPnts()[UW01] );

	/* Set rendering data */
	renderPtr->setValue( corners, R_FEA_WING_CORNERS );
}

/******************************************************
*
* Draw FEA Wing Section.
*
*******************************************************/
void wingSectionRenderer::draw()
{
	if ( !renderPtr )
		return;

	load();
	renderPtr->draw();
	clean();
}

/******************************************************
*
* Free Resources / Reset variables. 
* This Function is Called Internally.
*
*******************************************************/
void wingSectionRenderer::clean()
{
	if ( !renderPtr )
		return;

	//renderPtr->reset();
}

/******************************************************
*
* Turn on highlight.
*
*******************************************************/
void wingSectionRenderer::highlightOn()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( true, R_FEA_WING_HIGHLIGHT_FLAG );
}

/******************************************************
*
* Turn off highlight.
*
*******************************************************/
void wingSectionRenderer::highlightOff()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( false, R_FEA_WING_HIGHLIGHT_FLAG );
}

/******************************************************
*
* FEA Splice Line Renderer.
*
* This class wraps feaSpliceLineRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
spliceLineRenderer::spliceLineRenderer( FeaSpliceLine* spliceLine )
{
	spliceLinePtr = spliceLine;
	renderPtr = NULL;
}
spliceLineRenderer::~spliceLineRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void spliceLineRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaSpliceLineRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void spliceLineRenderer::load()
{
	if ( !renderPtr )
		return;

	vector< FeaSplice* > spliceVec = spliceLinePtr->m_SpliceVec;
	vector< double > sPos;
	for ( int i = 0; i < spliceVec.size(); i++ )
	{
		sPos.push_back( spliceVec[i]->m_Pos );
	}

	EndPoints endPoints;
	endPoints.start = spliceLinePtr->m_EndPnts[0];
	endPoints.end = spliceLinePtr->m_EndPnts[1];

	renderPtr->setValue( sPos, R_FEA_SPLICELINE_VEC_POS );
	renderPtr->setValue( spliceLinePtr->winWidth, R_FEA_SPLICELINE_WIN_WIDTH );
	renderPtr->setValue( spliceLinePtr->winHeight, R_FEA_SPLICELINE_WIN_HEIGHT );
	renderPtr->setValue( spliceLinePtr->m_WinXScale, R_FEA_SPLICELINE_WINXSCALE );
	renderPtr->setValue( endPoints, R_FEA_SPLICELINE_ENDPOINTS );
}

/******************************************************
*
* Draw Splice Line.
*
*******************************************************/
void spliceLineRenderer::draw()
{
	if ( !renderPtr )
		return;

	/* Load Rendering Data */
	load();
	
	/* Initialize some values */
	vector< FeaSplice* > spliceVec = spliceLinePtr->m_SpliceVec;
	double max_thick = spliceLinePtr->FindMaxThick();

	/* Draw Grid */
	renderPtr->drawGrid();
	
	/* Draw Control Points */
	for ( int i = 0; i < spliceVec.size(); i++ )
	{
		FeaSplice* sp = spliceVec[i];
		if ( sp == spliceLinePtr->GetEditSplice() )
		{
			/* Draw Control Point ( Edit ) */
			renderPtr->drawCtrlPntEdit( sp->m_Pos );
		}
		else if ( sp == spliceLinePtr->GetHighlightSplice() && spliceLinePtr->GetMode() == NORMAL_MODE )
		{
			/* Draw Control Point ( Highlight ) */
			renderPtr->drawCtrlPntHighlight( sp->m_Pos );
		}
		else
		{
			/* Draw Control Point ( Normal ) */
			renderPtr->drawCtrlPnt( sp->m_Pos );
		}
	}

	/* Draw Thick Dist */
	for ( int i = 0; i < ( int )spliceVec.size() - 1; i++ )
	{
		FeaSplice * sp0 = spliceVec[i];
		double tRatio0 = spliceVec[i]->m_Thick / max_thick;
		double tRatio1 = spliceVec[ i + 1 ]->m_Thick / max_thick;

		renderPtr->drawThick( spliceVec[i]->m_Pos, spliceVec[ i + 1 ]->m_Pos, tRatio0, tRatio1 );
	}

	/* Clean up */
	clean();
}

/******************************************************
*
* Draw Splice Line Main.
*
*******************************************************/
void spliceLineRenderer::drawMain()
{
	if ( !renderPtr )
		return;

	load();
	renderPtr->drawMain();
	clean();
}

/******************************************************
*
* Free Resources / Reset variables. 
* This Function is Called Internally.
*
*******************************************************/
void spliceLineRenderer::clean()
{
	if ( !renderPtr )
		return;

	renderPtr->reset();
}

/******************************************************
*
* FEA Skin Renderer.
*
* This class wraps feaSkinRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
skinRenderer::skinRenderer( FeaSkin* skin )
{
	skinPtr = skin;
	renderPtr = NULL;
}
skinRenderer::~skinRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void skinRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaSkinRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void skinRenderer::load()
{
	/* No use for now, leave it for the future */ 
}

/******************************************************
*
* Draw Skin.
*
*******************************************************/
void skinRenderer::draw()
{
	if ( !renderPtr )
		return;

	if ( !skinPtr->GetExportFlag() )
		return;

	load();

	for ( int i = 0; i < ( int )skinPtr->m_SpliceLineVec.size(); i++ )
	{
		if ( !i == skinPtr->m_CurrSpliceLineID )
		{
			highlightOff();
		}

		/* feaSkinRenderer sets up the rendering color */ 
		renderPtr->draw();

		/* feaSpliceLineRenderer draws shape */
		spliceLineRenderer renderer = spliceLineRenderer( skinPtr->m_SpliceLineVec[i] );
		renderer.init();
		renderer.drawMain();
	}

	clean();
}

/******************************************************
*
* Free Resources / Reset variables. 
* This Function is Called Internally.
*
*******************************************************/
void skinRenderer::clean()
{
	if ( !renderPtr )
		return;

	renderPtr->reset();
}

/******************************************************
*
* Turn on highlight.
*
*******************************************************/
void skinRenderer::highlightOn()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( true, R_FEA_SPLICELINE_SKIN_HIGHLIGHT );
}

/******************************************************
*
* Turn off highlight.
*
*******************************************************/
void skinRenderer::highlightOff()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( false, R_FEA_SPLICELINE_SKIN_HIGHLIGHT );
}

/******************************************************
*
* FEA Point Mass Renderer.
*
* This class wraps feaPointMassRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
pointMassRenderer::pointMassRenderer( FeaPointMass* pointMass )
{
	pointMassPtr = pointMass;
	renderPtr = NULL;
}
pointMassRenderer::~pointMassRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void pointMassRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaPointMassRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void pointMassRenderer::load()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( pointMassPtr->m_Pos, R_FEA_POINTMASS_POS );
	renderPtr->setValue( pointMassPtr->m_AttachPos, R_FEA_POINTMASS_ATTACHPOS );
}

/******************************************************
*
* Draw Point Mass.
*
*******************************************************/
void pointMassRenderer::draw()
{
	if ( !renderPtr )
		return;

	load();
	renderPtr->draw();
	clean();
}

/******************************************************
*
* Free Resources / Reset variables. 
* This Function is Called Internally.
*
*******************************************************/
void pointMassRenderer::clean()
{
	if ( !renderPtr )
		return;

	renderPtr->reset();
}

/******************************************************
*
* Turn on highlight.
*
*******************************************************/
void pointMassRenderer::highlightOn()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( true, R_FEA_POINTMASS_HIGHLIGHT );
}

/******************************************************
*
* Turn off highlight.
*
*******************************************************/
void pointMassRenderer::highlightOff()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( false, R_FEA_POINTMASS_HIGHLIGHT );
}

/******************************************************
*
* FEA Attachment Points Renderer.
*
* This class wraps feaAttachmentPointsRenderer class in 
* fea_mesh_renderer.cpp.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
attachmentPointsRenderer::attachmentPointsRenderer( FeaMeshMgr* feaMesh )
{
	feaPtr = feaMesh;
	renderPtr = NULL;
}
attachmentPointsRenderer::~attachmentPointsRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void attachmentPointsRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new feaAttachmentPointsRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void attachmentPointsRenderer::load()
{
	if ( !renderPtr )
		return;

	renderPtr->setValue( feaPtr->GetAttachmentPoints(), R_FEA_ATTACHPOINTS_VEC );
	renderPtr->setValue( feaPtr->GetClosestAttachPointIndex(), R_FEA_ATTACHPOINTS_CLOSESTATTACHPOINT_INDEX );
}

/******************************************************
*
* Draw Attachment Points.
*
*******************************************************/
void attachmentPointsRenderer::draw()
{
	if ( !renderPtr )
		return;

	load();
	renderPtr->draw();
	clean();
}

/******************************************************
*
* Free Resources / Reset variables. 
* This Function is Called Internally.
*
*******************************************************/
void attachmentPointsRenderer::clean()
{
	if ( !renderPtr )
		return;

	renderPtr->reset();
}