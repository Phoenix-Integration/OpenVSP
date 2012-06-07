#ifndef _FEA_RENDERER_H
#define _FEA_RENDERER_H

#include "FeaMeshMgr.h"
#include "fea_mesh_renderer.h"

/******************************************************
*
* FEA Slice Renderer.
*
*******************************************************/
class sliceRenderer
{
public:
	sliceRenderer( FeaSlice* feaSlice );
	virtual ~sliceRenderer();

public:
	virtual void init();
	virtual void draw();

public:
	void highlightOn();
	void highlightOff();

protected:
	virtual void load();
	virtual void clean();

protected:
	FeaSlice* slicePtr;
	feaSliceRenderer* renderPtr;
};

/******************************************************
*
* FEA Rib Renderer.
*
*******************************************************/
class ribRenderer : public sliceRenderer
{
public:
	ribRenderer( FeaSlice* feaSlice );
	virtual ~ribRenderer();

public:
	virtual void init();

protected:
	virtual void load();
};

/******************************************************
*
* FEA Spar Renderer.
*
*******************************************************/
class sparRenderer : public sliceRenderer
{
public:
	sparRenderer( FeaSlice* feaSlice );
	virtual ~sparRenderer();

public:
	virtual void init();

protected:
	virtual void load();
};

/******************************************************
*
* FEA Splice Line Renderer.
*
*******************************************************/
class spliceLineRenderer
{
public:
	spliceLineRenderer( FeaSpliceLine* spliceLine  );
	virtual ~spliceLineRenderer();

public:
	virtual void init();
	virtual void draw();
	virtual void drawMain();

protected:
	virtual void load();
	virtual void clean();

protected:
	FeaSpliceLine* spliceLinePtr;
	feaSpliceLineRenderer* renderPtr;
};

/******************************************************
*
* FEA Skin Renderer.
*
*******************************************************/
class skinRenderer
{
public:
	skinRenderer( FeaSkin* skin );
	virtual ~skinRenderer();

public:
	virtual void init();
	virtual void draw();

public:
	void highlightOn();
	void highlightOff();

protected:
	virtual void load();
	virtual void clean();

protected:
	FeaSkin* skinPtr;
	feaSkinRenderer* renderPtr;
};

/******************************************************
*
* FEA Wing Section Renderer.
*
*******************************************************/
class wingSectionRenderer
{
public:
	wingSectionRenderer( WingSection* wingSection );
	virtual ~wingSectionRenderer();

public:
	virtual void init();
	virtual void draw();

public:
	void highlightOn();
	void highlightOff();

protected:
	virtual void load();
	virtual void clean();

protected:
	WingSection* wingSectPtr;
	feaWingRenderer* renderPtr;
};

/******************************************************
*
* FEA Point Mass Renderer.
*
*******************************************************/
class pointMassRenderer
{
public:
	pointMassRenderer( FeaPointMass* pointMass );
	virtual ~pointMassRenderer();

public:
	virtual void init();
	virtual void draw();

public:
	void highlightOn();
	void highlightOff();

protected:
	virtual void load();
	virtual void clean();

protected:
	FeaPointMass* pointMassPtr;
	feaPointMassRenderer* renderPtr;
};

/******************************************************
*
* FEA Attachment Points Renderer.
*
*******************************************************/
class attachmentPointsRenderer
{
public:
	attachmentPointsRenderer( FeaMeshMgr* feaMesh );
	virtual ~attachmentPointsRenderer();

public:
	virtual void init();
	virtual void draw();

protected:
	virtual void load();
	virtual void clean();

protected:
	FeaMeshMgr* feaPtr;
	feaAttachmentPointsRenderer* renderPtr;
};

/******************************************************
*
* FEA Mesh Renderer.
*
* This class wraps fea_mesh_renderer in vsp_renderers
* library.
*
*******************************************************/
class feaRenderer
{
public:
	feaRenderer( FeaMeshMgr* feaMesh );
	virtual ~feaRenderer();

public:
	virtual void draw();

protected:
	virtual void drawRibs();
	virtual void drawSpars();
	virtual void drawWingSections();
	virtual void drawRibSparElements();
	virtual void drawSkin();
	virtual void drawPointMass();
	virtual void drawAttachmentPoints();

protected:
	FeaMeshMgr* feaPtr;
};
#endif