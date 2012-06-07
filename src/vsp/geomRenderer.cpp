
// TODO: Separate texturing related functions to vsprenderer lib.

#include "ms_wing.h"
#include "geomRenderer.h"

/******************************************************
*
* Geometry Renderer Class.
* 
* This class wraps geom_renderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
geomRenderer::geomRenderer(Geom* geom)
{
	geomPtr = geom;
	renderPtr = NULL;
}

geomRenderer::~geomRenderer()
{
	clean();

	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Geometry Renderer.  Call Before Any Draw
* Functions.
*
*******************************************************/
void geomRenderer::init()
{
	/* In case init is called multiple times,
		release resources from previous pointer. */
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new Renderer();
}

/******************************************************
*
* Load Geometry Info.
*
*******************************************************/
void geomRenderer::load()
{
	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	/* Clean Before Loading */
	clean();

	/* Load Rendering Data */

	/* load vertex data */
	for ( int i = 0; i < geomPtr->getNumSurf(); i++ )
	{
		/* get geom info */
		Xsec_surf* XSectionSurf = geomPtr->getXSectionSurfVec()[i];
		int num_xsecs = XSectionSurf->get_num_xsecs();
		int num_pnts = XSectionSurf->get_num_pnts();
		array_2d< vec3d > XSectionSurfVec = XSectionSurf->get_pnts_xsecs();

		/* set up renderer */
		XSecSurfRenderer* xsecRenderer = new XSecSurfRenderer();
		xsecRenderer->set_render_pnts( num_xsecs, num_pnts, XSectionSurfVec );
		xsecRendererVec.push_back( xsecRenderer );
	}
	renderPtr->setValue( xsecRendererVec, R_XSECTION_RENDERS );

	/* texture */
	renderPtr->setValue( geomPtr->getAppliedTexVec(), R_TEXTURE_VEC );

	/* bounding box */
	renderPtr->setValue( geomPtr->get_bnd_box(), R_BOUNDING_BOX );

	/* matrix and reflection style */
	renderPtr->setValue( geomPtr->getMat(), R_MODEL_MATRIX );
	renderPtr->setValue( geomPtr->getReflMat(), R_REFLECT_MATRIX );
	renderPtr->setValue( geomPtr->getSymCode(), R_SYM_CODE );

	/* color and lighting */
	renderPtr->setValue( geomPtr->getColor(), R_COLOR );
	renderPtr->setValue( geomPtr->getMaterialID(), R_MATERIAL_ID );

	/* flags */
	renderPtr->setValue( geomPtr->getRedFlag(), R_RED_FLAG );
	renderPtr->setValue( geomPtr->getYellowFlag(), R_YELLOW_FLAG );
	renderPtr->setValue( geomPtr->getNoShowFlag(), R_NOSHOW_FLAG );
	renderPtr->setValue( geomPtr->getDisplayFlag(), R_DISPLAY_FLAG );
	renderPtr->setValue( geomPtr->getFastDrawFlag(), R_FASTDRAW_FLAG );
}

/******************************************************
*
* Draw Geometry.
*
*******************************************************/
void geomRenderer::draw()
{
	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	/* Load Geometry Info */
	load();
	/* Render Surfaces */
	renderPtr->draw();
	/* Clean */
	clean();
}

/******************************************************
*
* Draw Geometry With Transparency.
*
*******************************************************/
void geomRenderer::drawAlpha()
{
	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	/* Load Geometry Info */
	load();
	/* Render Surfaces */
	renderPtr->drawAlpha();
	/* Clean Up */
	clean();
}

/******************************************************
*
* Draw Bounding Box.
*
*******************************************************/
void geomRenderer::draw_bbox()
{
	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	/* Set Bounding Box */
	renderPtr->setValue( geomPtr->get_bnd_box(), R_BOUNDING_BOX );

	/* Draw Bounding Box */
	renderPtr->draw_bbox();

	/* Clean Up */
	renderPtr->reset();
}

/******************************************************
*
* Draw Bounding Box With Highlight.
*
*******************************************************/
void geomRenderer::draw_highlight_boxes()
{
	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	/* Set Bounding Box */
	renderPtr->setValue( geomPtr->get_bnd_box(), R_BOUNDING_BOX );

	/* Set Flags */
	renderPtr->setValue( geomPtr->getRedFlag(), R_RED_FLAG );
	renderPtr->setValue( geomPtr->getYellowFlag(), R_YELLOW_FLAG );

	/* Draw Bounding Box With Highlight */
	renderPtr->draw_highlight_boxes();

	/* Clean UP */
	renderPtr->reset();
}

/******************************************************
*
* Draw Textures.
*
*******************************************************/
void geomRenderer::drawTextures( bool reflFlag )
{
	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	/* Load Geometry Info */
	load();

	/* Draw Textures */
	renderPtr->draw_textures( reflFlag );

	clean();

}

/******************************************************
*
* Clear Geometry Info.
*
*******************************************************/
void geomRenderer::clean()
{
	/* Clean Up */
	for ( int i = 0; i < ( int )xsecRendererVec.size(); i++ )
		delete xsecRendererVec[i];
	xsecRendererVec.clear();

	/* Make Sure Renderer Exist */
	if ( !renderPtr ) return;

	renderPtr->reset(); // set to default
}

/******************************************************
*
* Blank Geometry Renderer.
*
*******************************************************/
blankRenderer::blankRenderer( Geom* geom ) : geomRenderer( geom )
{
}
blankRenderer::~blankRenderer()
{
}
void blankRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new BlankGeomRenderer();
}

/******************************************************
*
* Multi Section Wing Geometry Renderer.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
mswingRenderer::mswingRenderer( Geom* geom ) : geomRenderer( geom )
{
}
mswingRenderer::~mswingRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize MSWingGeomRenderer.
*
*******************************************************/
void mswingRenderer::init()
{
	/* In case init is called multiple times,
	release resources from previous pointer. */
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new MSWingGeomRenderer();
}

/******************************************************
*
* Overloading load() From geomRenderer. Load Ms wing
* Specific Geom Info.
*
*******************************************************/
void mswingRenderer::load()
{
	/* Make Sure renderPtr Exist */
	if ( !renderPtr ) return;

	Ms_wing_geom* wingGeom = dynamic_cast< Ms_wing_geom* > ( geomPtr );

	assert( wingGeom );
	if ( !wingGeom ) return;

	/* Load General Info */
	geomRenderer::load();

	/* Load Multi Section Wing Specific Geom Info */
	renderPtr->setValue( wingGeom->get_highlight_type(), R_MSWING_HIGHLIGHT );
	renderPtr->setValue( wingGeom->get_sect_box(), R_MSWING_SEC_BOX );
}

/******************************************************
*
* Wing Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
wingRenderer::wingRenderer( Geom* geom ) : geomRenderer( geom )
{
}
wingRenderer::~wingRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize WingGeomRenderer.
*
*******************************************************/
void wingRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new WingGeomRenderer();
}

/******************************************************
*
* Pod Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
podRenderer::podRenderer( Geom * geom ) : geomRenderer( geom )
{
}
podRenderer::~podRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize PodGeomRenderer.
*
*******************************************************/
void podRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new PodGeomRenderer();
}

/******************************************************
*
* Fuse Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
fuseRenderer::fuseRenderer( Geom * geom ) : geomRenderer( geom )
{
}
fuseRenderer::~fuseRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize FuseGeomRenderer.
*
*******************************************************/
void fuseRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new FuseGeomRenderer();
}

/******************************************************
*
* Fuselage Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
fuselageRenderer::fuselageRenderer( Geom * geom ) : geomRenderer( geom )
{
}
fuselageRenderer::~fuselageRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize FuselageGeomRenderer.
*
*******************************************************/
void fuselageRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new FuselageGeomRenderer();
}

/******************************************************
*
* Havoc Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
havocRenderer::havocRenderer( Geom * geom ) : geomRenderer( geom )
{
}
havocRenderer::~havocRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize HavocGeomRenderer.
*
*******************************************************/
void havocRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new HavocGeomRenderer();
}

/******************************************************
*
* Duct Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
ductRenderer::ductRenderer( Geom * geom ) : geomRenderer( geom )
{
}
ductRenderer::~ductRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize DuctGeomRenderer.
*
*******************************************************/
void ductRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new DuctGeomRenderer();
}

/******************************************************
*
* External Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
extRenderer::extRenderer( Geom * geom ) : geomRenderer( geom )
{
}
extRenderer::~extRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize ExtGeomRenderer.
*
*******************************************************/
void extRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new ExtGeomRenderer();
}

/******************************************************
*
* Prop Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
propRenderer::propRenderer( Geom * geom ) : geomRenderer( geom )
{
}
propRenderer::~propRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize PropGeomRenderer.
*
*******************************************************/
void propRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new PropGeomRenderer();
}

/******************************************************
*
* Engine Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
engineRenderer::engineRenderer( Geom * geom ) : geomRenderer( geom )
{
}
engineRenderer::~engineRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize EngineGeomRenderer.
*
*******************************************************/
void engineRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new EngineGeomRenderer();
}

/******************************************************
*
* Cabin Layout Geometry renderer class.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
cabinlayoutRenderer::cabinlayoutRenderer( Geom * geom ) : geomRenderer( geom )
{
}
cabinlayoutRenderer::~cabinlayoutRenderer()
{
}

/******************************************************
*
* Overloading init() From geomRenderer.
* Initialize CabinLayoutGeomRenderer.
*
*******************************************************/
void cabinlayoutRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new CabinLayoutGeomRenderer();
}