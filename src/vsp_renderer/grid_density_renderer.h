#ifndef _GRID_DENSITY_RENDERER_H
#define _GRID_DENSITY_RENDERER_H

#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#endif

#ifdef _WIN32
#	include <Windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#	include <OpenGL\glu.h>
#else
#	include <gl\GL.h>
#endif

#include "vec3d.h"
#include "bbox.h"
#include "common.h"

/******************************************************
*
* Source Renderer.
*
*******************************************************/
class sourceRenderer
{
public:
	sourceRenderer();
	virtual ~sourceRenderer();

public:
	virtual void draw() {}
	virtual void draw_sphere( double rad, const vec3d& loc );

public:
	virtual void set_rad( double value ) {}
	virtual void set_box( bbox &value) {}

	virtual void set_loc( vec3d &value ) {}
	virtual void set_rad( double value1, double value2 ) {}
	virtual void set_end_pnts( const vec3d &value, const vec3d &value2 ) {}

	virtual void set_color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );

	virtual void reset();

protected:
	Color color;
};

/******************************************************
*
* Point Source Renderer.
*
*******************************************************/
class pSourceRenderer : public sourceRenderer
{
public:
	pSourceRenderer();
	virtual ~pSourceRenderer() {}

public:
	virtual void draw();

public:
	virtual void set_rad( double value ) { m_rad = value; }
	virtual void set_loc( vec3d &value ) { m_loc = value; }
	virtual void reset();

protected:
	double m_rad;
	vec3d m_loc;
};

/******************************************************
*
* Line Source Renderer.
*
*******************************************************/
class lSourceRenderer : public sourceRenderer
{
public:
	lSourceRenderer();
	virtual ~lSourceRenderer() {}

public:
	virtual void draw();

public:
	virtual void set_rad( double value1, double value2 );
	virtual void set_end_pnts( const vec3d &value, const vec3d &value2 );
	virtual void reset();

protected:
	double m_rad1;
	double m_rad2;
	vec3d m_loc1;
	vec3d m_loc2;
};

/******************************************************
*
* Box Source Renderer.
*
*******************************************************/
class bSourceRenderer : public sourceRenderer
{
public:
	bSourceRenderer();
	virtual ~bSourceRenderer() {}

public:
	virtual void draw();

public:
	virtual void set_box( bbox &value ) { m_box = value; }
	virtual void reset();

protected:
	bbox m_box;
};
#endif