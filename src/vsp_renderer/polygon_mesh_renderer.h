#ifndef _XSEC_SURF_RENDERER_H
#define _XSEC_SURF_RENDERER_H

#include "array.h"
#include "array_2d.h"
#include "vec3d.h"
#include "vec2d.h"
#include "util.h"
#include "texture.h"

/******************************************************
*
* Cross section surface renderer class.
*
*******************************************************/
class XSecSurfRenderer
{

/***** Constructor/Destructor *****/
public:

	XSecSurfRenderer();
	virtual ~XSecSurfRenderer();

public:

	/***** Draw Functions *****/
	void draw_wire();
	void draw_hidden();
	void draw_shaded();

	void draw_refl_wire( int sym_code_in );
	void draw_refl_hidden( int sym_code_in );
	void draw_refl_shaded( int sym_code_in );

	void draw_texture( AppliedTex& apTex );
	void draw_refl_texture( AppliedTex& apTex, int sym_code_in );

	/***** Get/Set Functions *****/
	int  get_num_xsecs() { return( num_xsecs ); }
	int  get_num_pnts() { return( num_pnts ); }
	bool get_draw_flag() { return draw_flag; }
	int  get_num_sect_interp() { return numSectInterp; }
	void get_xsec( int ixs, vector< vec3d > & pvec );
	void get_stringer( int ipnt, vector< vec3d > & pvec );

	void set_render_pnts( int num_xsecs_in, int num_pnts_in, array_2d<vec3d> pnts_xsecs_in );
	void set_draw_flag( bool flag );
	void set_highlight_xsec_id( int id ) { highlight_xsec_id = id; }
	void set_highlight_xsec_flag( int flag ) { highlight_xsec_flag = flag; }
	void set_highlight_xsec_color( vec3d color ) { highlight_xsec_color = color; }
	void set_sym_code( int sym_code ) { refl_pnts_xsecs_code = sym_code; }
	void set_num_sect_interp( int value ) { numSectInterp = value; };

	/***** Util Functions *****/
	void load_normals();
	void load_uw();
	void load_refl_pnts_xsecs();
	void load_refl_normals();

	void remap_texture( double uPos, double width, bool wrapFlag, vector< double > & uVec,
		vector< int > & uIndex, vector< double > & uRemap);

	void fast_draw_on() { fast_draw_flag = 1; }
	void fast_draw_off() { fast_draw_flag = 0; }

protected:

	/***** Protected Variables *****/
	int num_xsecs;
	int num_pnts;

	array_2d<vec3d> pnts_xsecs;
	array_2d<vec3d> hidden_surfs;
	array_2d<vec3d> normals;

	/* reflected */
	int refl_pnts_xsecs_code;
	int refl_hidden_code;
	int refl_normals_code;

	array_2d<vec3d> refl_pnts_xsecs;
	array_2d<vec3d> refl_hidden_surf;
	array_2d<vec3d> refl_normals;

	/* texture */
	array< double > uArray;
	array< double > wArray;

	/* highlight */
	int   highlight_xsec_id;
	vec3d highlight_xsec_color;
	
	/* flags */
	int fast_draw_flag;
	int highlight_xsec_flag;
	bool draw_flag;

	/* num of interpolated sections next to this one */
	int numSectInterp;
};
#endif