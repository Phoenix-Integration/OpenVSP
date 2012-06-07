#ifndef _GEOM_RENDERER_H
#define _GEOM_RENDERER_H

#include "geom.h"
#include "geom_renderer.h"

/******************************************************
*
* Geometry renderer class.
*
*******************************************************/
class geomRenderer
{
/* Constructor and Destructor */
public:
	geomRenderer( Geom* geom );
	virtual ~geomRenderer();

public:
	virtual void draw_highlight_boxes();
	virtual void draw_bbox();
	virtual void drawTextures( bool reflFlag );
	virtual void drawAlpha();
	virtual void draw();
	virtual void init();

protected:
	virtual void load();
	virtual void clean();

protected:
	Renderer* renderPtr;
	Geom* geomPtr;
	vector< XSecSurfRenderer* > xsecRendererVec;

};

/******************************************************
*
* Pod Geometry renderer class.
*
*******************************************************/
class podRenderer : public geomRenderer
{
public:
	podRenderer( Geom * geom );
	virtual ~podRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Fuse Geometry renderer class.
*
*******************************************************/
class fuseRenderer : public geomRenderer
{
public:
	fuseRenderer( Geom * geom );
	virtual ~fuseRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Havoc Geometry renderer class.
*
*******************************************************/
class havocRenderer : public geomRenderer
{
public:
	havocRenderer( Geom * geom );
	virtual ~havocRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Duct Geometry renderer class.
*
*******************************************************/
class ductRenderer : public geomRenderer
{
public:
	ductRenderer( Geom * geom );
	virtual ~ductRenderer();

public:
	virtual void init();
};

/******************************************************
*
* External Geometry renderer class.
*
*******************************************************/
class extRenderer : public geomRenderer
{
public:
	extRenderer( Geom * geom );
	virtual ~extRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Prop Geometry renderer class.
*
*******************************************************/
class propRenderer : public geomRenderer
{
public:
	propRenderer( Geom * geom );
	virtual ~propRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Engine Geometry renderer class.
*
*******************************************************/
class engineRenderer : public geomRenderer
{
public:
	engineRenderer( Geom * geom );
	virtual ~engineRenderer();

public:
	virtual void init();
};

/******************************************************
*
* HWB Geometry renderer class.
*
*******************************************************/
class hwbRenderer : public geomRenderer
{
public:
	hwbRenderer( Geom * geom );
	virtual ~hwbRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Fuselage Geometry renderer class.
*
*******************************************************/
class fuselageRenderer : public geomRenderer
{
public:
	fuselageRenderer( Geom * geom );
	virtual ~fuselageRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Cabin Layout Geometry renderer class.
*
*******************************************************/
class cabinlayoutRenderer : public geomRenderer
{
public:
	cabinlayoutRenderer( Geom * geom );
	virtual ~cabinlayoutRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Blank Geometry renderer class.
*
*******************************************************/
class blankRenderer : public geomRenderer
{
public:
	blankRenderer( Geom* geom );
	virtual ~blankRenderer();

public:
	virtual void init();
};

/******************************************************
*
* Multi Section Wing Geometry renderer class.
*
*******************************************************/
class mswingRenderer : public geomRenderer
{
public:
	mswingRenderer( Geom* geom );
	virtual ~mswingRenderer();

public :
	virtual void init();
	virtual void load();
};

/******************************************************
*
* Wing Geometry renderer class.
*
*******************************************************/
class wingRenderer : public geomRenderer
{
public:
	wingRenderer( Geom* geom );
	virtual ~wingRenderer();

public :
	virtual void init();
};

#endif