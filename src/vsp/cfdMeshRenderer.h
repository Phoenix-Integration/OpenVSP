#ifndef _CFD_RENDERER_H
#define _CFD_RENDERER_H

#include "CfdMeshMgr.h"
#include "cfd_mesh_renderer.h"

#include "GridDensity.h"
#include "grid_density_renderer.h"

/******************************************************
*
* Base Source Renderer Header.
*
*******************************************************/
class baseSourceRenderer
{
public:
	baseSourceRenderer( BaseSource* baseSource );
	virtual ~baseSourceRenderer();

public:
	virtual void init();
	virtual void draw();

public:
	virtual void setColor( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );

protected:
	virtual void load();
	virtual void clean();

protected:
	BaseSource* sourcePtr;
	sourceRenderer* renderPtr;
};

/******************************************************
*
* Point Source Renderer Header.
*
*******************************************************/
class pointSourceRenderer : public baseSourceRenderer
{
public:
	pointSourceRenderer( BaseSource* baseSource );
	virtual ~pointSourceRenderer();

public:
	virtual void init();

protected:
	virtual void load();
};

/******************************************************
*
* Line Source Renderer Header.
*
*******************************************************/
class lineSourceRenderer : public baseSourceRenderer
{
public:
	lineSourceRenderer( BaseSource* baseSource );
	virtual ~lineSourceRenderer();

public:
	virtual void init();

protected:
	virtual void load();
};

/******************************************************
*
* Box Source Renderer Header.
*
*******************************************************/
class boxSourceRenderer : public baseSourceRenderer
{
public:
	boxSourceRenderer( BaseSource* baseSource );
	virtual ~boxSourceRenderer();

public:
	virtual void init();

protected:
	virtual void load();
};

/******************************************************
*
* Grid Density Renderer Header.
*
* This class wraps grid_density_renderer in vsp_renderers
* library.
*
*******************************************************/
class gridDensityRenderer
{
public:
	gridDensityRenderer( GridDensity* gridDensity );
	virtual ~gridDensityRenderer();

public:
	virtual void draw();

public:
	virtual void setValue( BaseSource* value, int ID );

protected:
	virtual void load();
	virtual void clean();

private:
	baseSourceRenderer* create( BaseSource* source );

protected:
	GridDensity* gdPtr;
	vector< baseSourceRenderer* > renderPtrs;
	BaseSource* currentSource;

private:
	vector< baseSourceRenderer* > resources;
};

/******************************************************
*
* CFD Mesh Renderer Header.
*
* This class wraps cfd_mesh_renderer in vsp_renderers
* library.
*
*******************************************************/
class cfdRenderer
{
public:
	cfdRenderer( CfdMeshMgr* cfdMesh );
	virtual ~cfdRenderer();

public:
	virtual void init();
	virtual void draw();

public:
	virtual void setValue( gridDensityRenderer* value, int ID );

protected:
	virtual void load();
	virtual void clean();

private:
	tMeshRenderer* create( Surf* surface );
	vector< tMeshRenderer* > create( vector< Surf* > surfaces );

protected:
	CfdMeshMgr* cfdMeshPtr;
	cfdMeshRenderer* renderPtr;
	gridDensityRenderer* gdRenderPtr;

private:
	vector< tMeshRenderer* > resources;
};

#endif