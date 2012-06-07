
// FIXME: isShown flag is hardcoded as true. (see cfdRenderer::draw()).

#include "cfdMeshRenderer.h"
#include "common.h"

/******************************************************
*
* CFD Mesh Renderer.
*
* This class wraps cfd_mesh_renderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
cfdRenderer::cfdRenderer( CfdMeshMgr* cfdMesh )
{
	cfdMeshPtr = cfdMesh;
	renderPtr = NULL;
	gdRenderPtr= NULL;
}
cfdRenderer::~cfdRenderer()
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
void cfdRenderer::init()
{
	/* Just In Case init() is Called Multiple Times */
	if ( renderPtr )
		delete renderPtr;

	/* Initialize Renderer */
	renderPtr = new cfdMeshRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void cfdRenderer::load()
{
	/* Make Sure Init is Called */
	if ( !renderPtr )
		return;

	/* Free Resources Before Load */
	clean();

	/* Load Rendering Data */
	renderPtr->setValue( create( cfdMeshPtr->GetSurfVec() ), R_CFD_TMESH_RENDERS );

	/* Load Grid Density */
	if ( gdRenderPtr )
		gdRenderPtr->setValue( cfdMeshPtr->GetCurrSource(), R_GD_CURR_SOURCE );
}

/******************************************************
*
* Draw CFD Mesh.
*
*******************************************************/
void cfdRenderer::draw()
{
	if ( !renderPtr )
		return;

	// FIXME: Hard coded isShown flag!!!
	bool isShown = true;
	if ( !isShown )
		return;

	load();

	if ( cfdMeshPtr->GetDrawSourceFlag() )
	{
		/* Draw Sources */
		if ( gdRenderPtr )
			gdRenderPtr->draw();
	}

	if ( cfdMeshPtr->GetDrawMeshFlag() )
	{
		/* Draw CFD Mesh */
		renderPtr->draw();
	}

	clean();
}

/******************************************************
*
* Free Resources. 
* This Function is Called Internally.
*
*******************************************************/
void cfdRenderer::clean()
{
	/* Reset Variables to Default */
	if ( renderPtr )
		renderPtr->reset();

	/* Free Resources */
	for ( int i = 0; i < resources.size(); i++ )
	{
		delete resources[i];
	}
	resources.clear();
}

/******************************************************
*
* Set Grid Density Pointer.
*
*******************************************************/
void cfdRenderer::setValue( gridDensityRenderer* value, int ID )
{
	if ( !value )
		return;

	switch ( ID )
	{
		case R_CFD_GDENSITY_RENDER:
			gdRenderPtr = value;
			break;
	}
}

/******************************************************
*
* Generate tMeshRenderer base on Surf.
* Helper Class For Getting Vertex Info From Surf.
*
*******************************************************/
tMeshRenderer* cfdRenderer::create( Surf* surface )
{
	vector< vec3d > vertexInfo;

	vector< vec3d > surfInfo = surface->GetMesh()->GetSimpPntVec();
	for ( int i = 0; i < surface->GetMesh()->GetSimpTriVec().size(); i++ )
	{
		SimpTri* tri = &surface->GetMesh()->GetSimpTriVec()[i];
		vertexInfo.push_back( vec3d( surfInfo[tri->ind0].x(), surfInfo[tri->ind0].y(), surfInfo[tri->ind0].z() ) );
		vertexInfo.push_back( vec3d( surfInfo[tri->ind1].x(), surfInfo[tri->ind1].y(), surfInfo[tri->ind1].z() ) );
		vertexInfo.push_back( vec3d( surfInfo[tri->ind2].x(), surfInfo[tri->ind2].y(), surfInfo[tri->ind2].z() ) );
	}
	tMeshRenderer* renderer = new tMeshRenderer();
	renderer->setValue( vertexInfo, R_TMESH_VERTICES );

	/* Store resource pointer so we can delete them later */
	resources.push_back( renderer );

	return renderer;
}

/******************************************************
*
* Generate tMeshRenderer base on Surf ( multiple ).
* Helper Class For Getting Vertex Info From Surf.
*
*******************************************************/
vector< tMeshRenderer* > cfdRenderer::create( vector< Surf* > surfaces )
{
	vector< tMeshRenderer* > renderers;
	for ( int i = 0; i < surfaces.size(); i++ )
	{
		renderers.push_back( create( surfaces[i] ) );
	}
	return renderers;
}

/******************************************************
*
* Grid Density Renderer.
*
* This class wraps all sources ( box, line, point ) 
* in vsp_renderers library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
gridDensityRenderer::gridDensityRenderer( GridDensity* gridDensity )
{
	gdPtr = gridDensity;
	currentSource = NULL;
}
gridDensityRenderer::~gridDensityRenderer()
{
	clean();
	renderPtrs.clear();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void gridDensityRenderer::load()
{
	/* Clean before Load */
	if ( renderPtrs.size() > 0 )
	{
		clean();
		renderPtrs.clear();
	}

	vector< BaseSource* > sourcePtrs = gdPtr->GetSources();

	/* Initialize Renderer Pointers */
	for ( int i = 0; i < sourcePtrs.size(); i++ )
	{
		/* Generate Renderer from Source */
		baseSourceRenderer* renderer;
		renderer = create ( sourcePtrs[i] );

		/* Initialize Renderer */
		renderer->init();
		renderer->setColor( 100, 100, 100, 255 );

		/* Change Color if Currently Selected */
		if ( currentSource == sourcePtrs[i] )
		{
			renderer->setColor( 255, 100, 0, 255 );
		}

		renderPtrs.push_back( renderer );
	}
}

/******************************************************
*
* Free Resources / Reset Variables. 
* This Function is Called Internally.
*
*******************************************************/
void gridDensityRenderer::clean()
{
	/* Free Resources */
	for ( int i = 0; i < resources.size(); i++ )
	{
		delete resources[i];
	}
	resources.clear();

	/* Clear Renderers */
	renderPtrs.clear();
}

/******************************************************
*
* Draw Grid Density. 
*
*******************************************************/
void gridDensityRenderer::draw()
{
	/* Load source renderers */
	load();

	/* Draw sources */
	for ( int i = 0; i < renderPtrs.size(); i++ )
	{
		renderPtrs[i]->draw();
	}

	/* Clean up */
	clean();
}

/******************************************************
*
* Set BaseSource Pointers. 
*
*******************************************************/
void gridDensityRenderer::setValue( BaseSource* value, int ID )
{
	if ( !value )
		return;

	switch ( ID )
	{
		case R_GD_CURR_SOURCE:
			currentSource = value;
			break;
	}
}

/******************************************************
*
* Generate baseSourceRenderer base on Source.
* Helper Class For Getting Type Info From Source.
*
*******************************************************/
baseSourceRenderer* gridDensityRenderer::create( BaseSource* source )
{
	baseSourceRenderer* renderer;

	/* Identify BaseSource Type */
	if ( dynamic_cast< PointSource* > ( source ) )
		renderer = new pointSourceRenderer( source );

	else if ( dynamic_cast< LineSource* > ( source ) )
		renderer = new lineSourceRenderer( source );

	else if ( dynamic_cast< BoxSource* > ( source ) )
		renderer = new boxSourceRenderer( source );

	else
	{
		/* Shouldn't reach here */
		/* Something must went wrong */
		assert( false );
		renderer = new baseSourceRenderer( source );
	}

	/* Store resource pointer so we can delete them later */
	resources.push_back( renderer );

	return renderer;
}

/******************************************************
*
* Base Source Renderer.
*
* This class wraps sourceRenderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
baseSourceRenderer::baseSourceRenderer( BaseSource* baseSource )
{
	sourcePtr = baseSource; 
	renderPtr = NULL;
}
baseSourceRenderer::~baseSourceRenderer()
{
	if ( renderPtr )
		delete renderPtr;
}

/******************************************************
*
* Initialize Renderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void baseSourceRenderer::init()
{
	/* Just in case init() is called multiple times */
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new sourceRenderer();
}

/******************************************************
*
* Load Rendering Info. 
* This Function is Called Internally.
*
*******************************************************/
void baseSourceRenderer::load()
{
}

/******************************************************
*
* Free Resources / Reset Variables. 
* This Function is Called Internally.
*
*******************************************************/
void baseSourceRenderer::clean()
{
	if ( renderPtr )
		renderPtr->reset();
}

/******************************************************
*
* Draw Source. 
*
*******************************************************/
void baseSourceRenderer::draw()
{
	if ( !renderPtr )
		return;

	load();
	renderPtr->draw();
	clean();
}

/******************************************************
*
* Set Color. 
*
*******************************************************/
void baseSourceRenderer::setColor( unsigned char red, 
											  unsigned char green, 
											  unsigned char blue, 
											  unsigned char alpha )
{
	renderPtr->set_color( red, green, blue, alpha );
}

/******************************************************
*
* Point Source Renderer.
*
* This class wraps pSourceRenderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
pointSourceRenderer::pointSourceRenderer( BaseSource* baseSource ) : baseSourceRenderer( baseSource )
{
}
pointSourceRenderer::~pointSourceRenderer()
{
}

/******************************************************
*
* Overloading init() from baseSourceRenderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void pointSourceRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new pSourceRenderer();
}

/******************************************************
*
* Overloading load() from baseSourceRenderer. 
* This Function is Called Internally.
*
*******************************************************/
void pointSourceRenderer::load()
{
	if ( !renderPtr )
		return;

	renderPtr->set_loc( ( ( PointSource* )sourcePtr )->GetLoc() );
	renderPtr->set_rad( ( ( PointSource* )sourcePtr )->GetRad() );
}

/******************************************************
*
* Line Source Renderer.
*
* This class wraps lSourceRenderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
lineSourceRenderer::lineSourceRenderer( BaseSource* baseSource ) : baseSourceRenderer( baseSource )
{
}
lineSourceRenderer::~lineSourceRenderer()
{
}

/******************************************************
*
* Overloading init() from baseSourceRenderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void lineSourceRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new lSourceRenderer();
}

/******************************************************
*
* Overloading load() from baseSourceRenderer. 
* This Function is Called Internally.
*
*******************************************************/
void lineSourceRenderer::load()
{
	if ( !renderPtr )
		return;

	renderPtr->set_rad( ( ( LineSource* )sourcePtr )->GetRad(),
		( ( LineSource* )sourcePtr )->GetRad2() );
	renderPtr->set_end_pnts( ( ( LineSource* )sourcePtr )->GetEndPnt1(),
		( ( LineSource* )sourcePtr )->GetEndPnt2() );
}

/******************************************************
*
* Box Source Renderer.
*
* This class wraps bSourceRenderer in vsp_renderers
* library.
*
*******************************************************/
/******************************************************
*
* Constructor / Destructor.
*
*******************************************************/
boxSourceRenderer::boxSourceRenderer( BaseSource* baseSource ) : baseSourceRenderer( baseSource )
{
}
boxSourceRenderer::~boxSourceRenderer()
{
}

/******************************************************
*
* Overloading init() from baseSourceRenderer. 
* Must Call Before Any Draw Functions.
*
*******************************************************/
void boxSourceRenderer::init()
{
	if ( renderPtr )
		delete renderPtr;

	renderPtr = new bSourceRenderer();
}

/******************************************************
*
* Overloading load() from baseSourceRenderer. 
* This Function is Called Internally.
*
*******************************************************/
void boxSourceRenderer::load()
{
	if ( !renderPtr )
		return;

	renderPtr->set_box( ( ( BoxSource* )sourcePtr )->GetBBox() );
}
