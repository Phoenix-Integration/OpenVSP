/******************************************************
*
* Common holds common structures/variables that can be 
* used across multiple project.
*
*******************************************************/
#ifndef _Common_H
#define _Common_H

/* SYM */
#define NO_SYM 0
#define XY_SYM 1
#define XZ_SYM 2
#define YZ_SYM 3

#define POS_ATTACH_NONE		0
#define POS_ATTACH_FIXED	1
#define POS_ATTACH_UV		2 
#define POS_ATTACH_MATRIX	3

#define GEOM_WIRE_FLAG		0
#define GEOM_HIDDEN_FLAG	1
#define GEOM_SHADE_FLAG		2
#define GEOM_TEXTURE_FLAG	3

//==== Update Tags ====//
#define UPD_XFORM					101010			// General Update Tags	
#define UPD_NUM_PNT_XSEC		(UPD_XFORM+1)
#define UPD_POD					(UPD_XFORM+2)	// Pod Update Tags
#define UPD_MW_ALL				(UPD_XFORM+3)	// Wing Update Tags
#define UPD_MW_LOC_ROT			(UPD_XFORM+4)
#define UPD_MW_NUM_PNTS			(UPD_XFORM+5)
#define UPD_MW_AIRFOIL			(UPD_XFORM+6)
#define UPD_MW_CONT_SURF		(UPD_XFORM+7)
#define UPD_MW_SWEEP_LOC		(UPD_XFORM+8)
#define UPD_FUSE_XSEC			(UPD_XFORM+9)	// Fuse Update Tags
#define UPD_FUSE_BODY			(UPD_XFORM+10)
#define UPD_HAVOC					(UPD_XFORM+11)	// Havoc
#define UPD_EDIT_CRV				(UPD_XFORM+12)	// Edit Curve

#define NOSE 0
#define AFT  1

//===== Define Computed Parameters =====
#define NUM_COMP_PARMS	6
#define TOTAL_LENGTH		0
#define NOSE_ASPECT		1
#define AFT_ASPECT		2
#define SEP_XSEC_AREA	3
#define MAX_XSEC_AREA	4
#define MAX_XSEC_WIDTH  5

//===== Define Computed Slice Parameters =====
#define NUM_SLICE_PARMS 6
#define X_LOC				0
#define BURY_FLAG			1
#define AREA				2
#define ASPECT				3
#define CAMBER				4
#define CORNER_RAD		5

/* Rendering Modes */
#define GEOM_WIRE_FLAG		0
#define GEOM_HIDDEN_FLAG	1
#define GEOM_SHADE_FLAG		2
#define GEOM_TEXTURE_FLAG	3

/* Multi Sectioned Wing Highlight */
#define MSW_HIGHLIGHT_NONE		0
#define MSW_HIGHLIGHT_SECT		1
#define MSW_HIGHLIGHT_JOINT	2
#define MSW_HIGHLIGHT_FOIL		3

enum 
{ 
	UP_SKIN_EDIT, 
	LOW_SKIN_EDIT, 
	SPAR_EDIT, 
	RIB_EDIT, 
	POINT_MASS_EDIT, 
};

enum { 
	UW00, 
	UW10, 
	UW01, 
	UW11, 
	NUM_CORNER_PNTS,
};

enum { 
	NORMAL_MODE, 
	ADD_MODE, 
	DEL_MODE, 
	EDIT_MODE, 
};

/* === Variable IDs === */
enum
{
	/* General Rendering Variables */
	R_MATERIAL_ID,
	R_SYM_CODE,
	R_RED_FLAG,
	R_YELLOW_FLAG,
	R_NOSHOW_FLAG,
	R_DISPLAY_FLAG,
	R_FASTDRAW_FLAG,
	R_MODEL_MATRIX,
	R_REFLECT_MATRIX,
	R_BOUNDING_BOX,
	R_XSECTION_RENDERS,
	R_TEXTURE_VEC,
	R_COLOR,

	/* MSWing Rendering Variables */
	R_MSWING_HIGHLIGHT,
	R_MSWING_SEC_BOX,

	/* Fuselage Rendering Variables */
	R_CURR_XSEC_NUM,
	R_NUM_SEC_INTERP,
	R_FUSELAGE_XSEC_RENDERS,
	R_FUSELAGE_CTRL_PNTS,
	R_ACTIVE_CTRL_PNT_ID,

	/* CFD Mesh Variables */
	R_CFD_TMESH_RENDERS,
	R_CFD_GDENSITY_RENDER,

	/* Grid Density Variables */
	R_GD_CURR_SOURCE,				// currentSource

	/* Triangle Mesh */
	R_TMESH_VERTICES,

	/* FEA Renderer */
	R_FEA_SLICE_HIGHLIGHT_FLAG,
	R_FEA_SLICE_PLANEDATA,
	R_FEA_SLICE_UPPER_PNTS,
	R_FEA_SLICE_LOWER_PNTS,
	R_FEA_SLICE_UPPER_END_PNTS,
	R_FEA_SLICE_UPPER_CHAIN_PNTS,
	R_FEA_SLICE_LOWER_CHAIN_PNTS,
	R_FEA_WING_CORNERS,
	R_FEA_WING_HIGHLIGHT_FLAG,
	R_FEA_SPLICELINE_WIN_HEIGHT,
	R_FEA_SPLICELINE_WIN_WIDTH,
	R_FEA_SPLICELINE_WINXSCALE,
	R_FEA_SPLICELINE_ENDPOINTS,
	R_FEA_SPLICELINE_VEC_POS,
	R_FEA_SPLICELINE_SKIN_HIGHLIGHT,
	R_FEA_ELEMENT_CORNER_PNTS,
	R_FEA_POINTMASS_POS,
	R_FEA_POINTMASS_ATTACHPOS,
	R_FEA_POINTMASS_HIGHLIGHT,
	R_FEA_ATTACHPOINTS_VEC,
	R_FEA_ATTACHPOINTS_CLOSESTATTACHPOINT_INDEX,
};

enum CullingMode
{
	FRONT,
	BACK,
	FRONT_AND_BACK,
};

class Mask
{
public:
	enum flag
	{

	};
};

/* === Common Structures === */

class ControlPnt
{
public:
	enum { TOP, BOTTOM, SIDE, };
	
	int xSecID;
	int pntID;

	vec3d pnt3d;
	vec2d pnt2d;
};

class Color
{
public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

class EndPoints
{
public:
	vec3d start;
	vec3d end;
};

#endif