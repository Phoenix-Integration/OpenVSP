#ifndef _FEA_MESH_RENDERER_H
#define _FEA_MESH_RENDERER_H

#include "vec3d.h"
#include "common.h"

/******************************************************
*
* FEA Slice Renderer.
*
*******************************************************/
class feaSliceRenderer
{
public:
	feaSliceRenderer();
	virtual ~feaSliceRenderer();

public:
	virtual void draw();
	virtual void drawSlicePlane();

public:
	virtual void setValue( vector< vec3d > value, int ID );
	virtual void setValue( EndPoints value, int ID );
	virtual void setValue( bool value, int ID );
	virtual void reset();

protected:
	bool highlight;

	vector< vec3d > planeData;
	vector< vec3d > upperPnts;
	vector< vec3d > lowerPnts;

	EndPoints upperEndPnts;
	EndPoints upperChainPnts;
	EndPoints lowerChainPnts;
};

/******************************************************
*
* FEA Rib Renderer.
*
*******************************************************/
class feaRibRenderer : public feaSliceRenderer
{
public:
	feaRibRenderer();
	virtual ~feaRibRenderer();

public:
	virtual void draw();
};

/******************************************************
*
* FEA Spar Renderer.
*
*******************************************************/
class feaSparRenderer : public feaSliceRenderer
{
public:
	feaSparRenderer();
	virtual ~feaSparRenderer();

public:
	virtual void draw();
};

/******************************************************
*
* Wing Section Renderer.
*
*******************************************************/
class feaWingRenderer
{
public:
	feaWingRenderer();
	virtual ~feaWingRenderer();

public:
	virtual void draw();

public:
	virtual void setValue( bool value, int ID );
	virtual void setValue( vector< vec3d > value, int ID );
	virtual void reset();

protected:
	bool highlight;
	vector< vec3d > corners;
};

/******************************************************
*
* Splice Line Renderer.
*
*******************************************************/
class feaSpliceLineRenderer
{
public:
	feaSpliceLineRenderer();
	virtual ~feaSpliceLineRenderer();

public:
	virtual void drawGrid();
	virtual void drawCtrlPnt( double pos );
	virtual void drawCtrlPntEdit( double pos );
	virtual void drawCtrlPntHighlight( double pos );
	virtual void drawThick( double currPos, double nextPos, double t0, double t1 );
	virtual void drawMain();

public:
	virtual void setValue( EndPoints value, int ID );
	virtual void setValue( vector< double > value, int ID );
	virtual void setValue( double value, int ID );
	virtual void reset();

private:
	void drawCtrlPntHelper( double sPos, double ht );

protected:
	EndPoints endPoints;
	vector< double > spliceVecPos;
	double winXScale;
	double winHeight;
	double winWidth;
};

/******************************************************
*
* Skin Renderer.
*
*******************************************************/
class feaSkinRenderer
{
public:
	feaSkinRenderer();
	virtual ~feaSkinRenderer();

public:
	virtual void draw();

public:
	virtual void setValue( bool value, int ID );
	virtual void reset();

protected:
	bool highlight;
};

/******************************************************
*
* Element Renderer.
*
*******************************************************/
class feaElementRenderer
{
public:
	feaElementRenderer();
	virtual ~feaElementRenderer();

public:
	virtual void draw();
	virtual void drawWire();

public:
	virtual void setValue( vector< vec3d > value, int ID );
	virtual void reset();

public:
	virtual void set_Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
	virtual void set_Point_Size( double size );
	virtual void set_Line_Width( double size );
	virtual void enableCullFace( CullingMode mode );
	virtual void disableCullFace();

protected:
	vector< vec3d > cornerPnts;
};

class feaPointMassRenderer
{
public:
	feaPointMassRenderer();
	virtual ~feaPointMassRenderer();

public:
	virtual void draw();

public:
	virtual void setValue( vec3d value, int ID );
	virtual void setValue( bool value, int ID );
	virtual void reset();

protected:
	bool highlight;
	vec3d pos;
	vec3d attachPos;
};

/******************************************************
*
* Attachment Points Renderer.
*
*******************************************************/
class feaAttachmentPointsRenderer
{
public:
	feaAttachmentPointsRenderer();
	virtual ~feaAttachmentPointsRenderer();

public:
	virtual void draw();

public:
	virtual void setValue( vector< vec3d > value, int ID );
	virtual void setValue( int value, int ID );
	virtual void reset();

protected:
	vector< vec3d > attachPoints;
	int closestAttachPointIndex;
};

#endif