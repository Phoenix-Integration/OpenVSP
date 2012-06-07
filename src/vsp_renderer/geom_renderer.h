#ifndef _RENDERER_H
#define _RENDERER_H

#include "vec3d.h"
#include "matrix.h"
#include "bbox.h"
#include "texture.h"
#include "polygon_mesh_renderer.h"
#include "common.h"

/******************************************************
*
* Renderer class.
*
*******************************************************/
class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

public:
	virtual void draw();
	virtual void drawAlpha();
	virtual void draw_bbox();
	virtual void draw_bbox( bbox &box );
	virtual void draw_highlight_boxes();
	virtual void draw_textures( bool reflFlag );
	virtual void storeModelMatrix(Matrix* viewMatrix, Matrix* modelMatrix, Matrix* reflMatrix, float* model_mat, float* reflect_mat);

public:
	virtual void setValue( int value, int ID );
	virtual void setValue( float* value, int ID );
	virtual void setValue( bbox value, int ID );
	virtual void setValue( vector<XSecSurfRenderer*> value, int ID );
	virtual void setValue( vector<AppliedTex> value, int ID );
	virtual void setValue( vec3d value, int ID );
	virtual void reset();

protected:
	virtual void draw_wire();
	virtual void draw_wire_refl();
	virtual void draw_shade();
	virtual void draw_shade_refl();
	virtual void draw_hidden();
	virtual void draw_hidden_refl();

protected:
	int materialID, sym_code, redFlag, yellowFlag, noshowFlag, displayFlag, fastDrawFlag;
	float* model_mat;
	float* reflect_mat;
	bbox bnd_box_xform;
	vector<XSecSurfRenderer*> surfVec;
	vector<AppliedTex> appTexVec;
	vec3d color;
};

/******************************************************
*
* Blank Geometry renderer class.
*
*******************************************************/
class BlankGeomRenderer : public Renderer
{
public:
	BlankGeomRenderer();
	virtual ~BlankGeomRenderer();

public:
	virtual void draw();
};

/******************************************************
*
* Cabin Layout Geometry renderer class.
*
*******************************************************/
class CabinLayoutGeomRenderer : public Renderer
{
public:
	CabinLayoutGeomRenderer();
	virtual ~CabinLayoutGeomRenderer();
};

/******************************************************
*
* Duct Geometry renderer class.
*
*******************************************************/
class DuctGeomRenderer : public Renderer
{
public:
	DuctGeomRenderer();
	virtual ~DuctGeomRenderer();
};

/******************************************************
*
* Engine Geometry renderer class.
*
*******************************************************/
class EngineGeomRenderer : public Renderer
{
public:
	EngineGeomRenderer();
	virtual ~EngineGeomRenderer();
};

/******************************************************
*
* Ext Geometry renderer class.
*
*******************************************************/
class ExtGeomRenderer : public Renderer
{
public:
	ExtGeomRenderer();
	virtual ~ExtGeomRenderer();
};

/******************************************************
*
* Fuse Geometry renderer class.
*
*******************************************************/
class FuseGeomRenderer : public Renderer
{
public:
	FuseGeomRenderer();
	virtual ~FuseGeomRenderer();
};

/******************************************************
*
* Fuselage Geometry renderer class.
*
*******************************************************/
class FuselageGeomRenderer : public Renderer
{
public:
	FuselageGeomRenderer();
	virtual ~FuselageGeomRenderer();

public:
	virtual void draw_wire();
	virtual void draw_control_points();

public:
//	virtual void set( int value, int ID );
	virtual void setValue( vector< ControlPnt* > value, int ID );

protected:
	int curr_xsec_num;
	int active_ctrl_pnt_id;
	XSecSurfRenderer* primarySurf;
	vector< ControlPnt* > cPntVec;
};

/******************************************************
*
* Havoc Geometry renderer class.
*
*******************************************************/
class HavocGeomRenderer : public Renderer
{
public:
	HavocGeomRenderer();
	virtual ~HavocGeomRenderer();
};

/******************************************************
*
* HWB Geometry renderer class.
*
*******************************************************/
class HWBGeomRenderer : public Renderer
{
public:
	HWBGeomRenderer();
	virtual ~HWBGeomRenderer();
};

/******************************************************
*
* Mesh Geometry renderer class.
*
*******************************************************/
class MeshGeomRenderer : public Renderer
{
public:
	MeshGeomRenderer();
	virtual ~MeshGeomRenderer();

//public:
//	virtual void draw();
//	virtual void drawAlpha();
};

/******************************************************
*
* MS Wing Geometry renderer class.
*
*******************************************************/
class MSWingGeomRenderer : public Renderer
{
public:
	MSWingGeomRenderer();
	virtual ~MSWingGeomRenderer();

public:
	virtual void draw();

public:
	virtual void setValue( int value, int ID );
	virtual void setValue( bbox value, int ID );
	virtual void reset();

protected:
	virtual void draw_sect_box();

protected:
	bbox section_Box;
	int highLightType;
};

/******************************************************
*
* Pod Geometry renderer class.
*
*******************************************************/
class PodGeomRenderer : public Renderer
{
public:
	PodGeomRenderer();
	virtual ~PodGeomRenderer();
};

/******************************************************
*
* Prop Geometry renderer class.
*
*******************************************************/
class PropGeomRenderer : public Renderer
{
public:
	PropGeomRenderer();
	virtual ~PropGeomRenderer();

//public:
//	virtual void draw();
};

/******************************************************
*
* Vor Geometry renderer class.
*
*******************************************************/
class VorGeomRenderer : public Renderer
{
public:
	VorGeomRenderer();
	virtual ~VorGeomRenderer();

//public:
//	virtual void draw();
};

/******************************************************
*
* Wing Geometry renderer class.
*
*******************************************************/
class WingGeomRenderer : public Renderer
{
public:
	WingGeomRenderer();
	virtual ~WingGeomRenderer();

public:
	virtual void draw();
	virtual void drawAlpha();

protected:
	virtual void draw_shade();
	virtual void draw_shade_refl();
};

/******************************************************
*
* X Section Geometry renderer class.
*
*******************************************************/
class XSecGeomRenderer : public Renderer
{
public:
	XSecGeomRenderer();
	virtual ~XSecGeomRenderer();
};
#endif