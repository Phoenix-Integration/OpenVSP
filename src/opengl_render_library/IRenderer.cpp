#include "IRenderer.h"

IRenderer::IRenderer()
{
	rp_draw3D.mode.blendMode.enabled = true;
	rp_draw3D.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_draw3D.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	rp_draw3D.mode.depthMaskMode.enabled = true;
	rp_draw3D.mode.depthTestMode.enabled = true;
	rp_draw3D.mode.depthTestMode.depthfunc.func = R_LESS;
	rp_draw3D.mode.normalizeMode.enabled = true;

	rp_draw2D.mode.blendMode.enabled = true;
	rp_draw2D.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_draw2D.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	rp_draw2D.mode.depthMaskMode.enabled = true;

	/* Customized Render Properties */ 

	/* Render Properties for drawing hidden */
	rp_hidden.mode.polygonOffsetMode.enabled = true;
	rp_hidden.mode.polygonOffsetMode.polygonOffset.factor = 2.0;
	rp_hidden.mode.polygonOffsetMode.polygonOffset.units = 1.0;
	rp_hidden.mode.depthMaskMode.enabled = true;
	rp_hidden.mode.depthTestMode.enabled = true;
	rp_hidden.mode.depthTestMode.depthfunc.func = R_LESS;

	/* Render Properties for drawing shaded */
	rp_shaded.mode.blendMode.enabled = true;
	rp_shaded.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_shaded.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	rp_shaded.mode.normalizeMode.enabled = true;
	rp_shaded.mode.lightingMode.enabled = true;
	rp_shaded.mode.cullFaceMode.enabled = true;
	rp_shaded.mode.cullFaceMode.cullface.mode = R_BACK;
	rp_shaded.mode.depthMaskMode.enabled = true;
	rp_shaded.mode.depthTestMode.enabled = true;
	rp_shaded.mode.depthTestMode.depthfunc.func = R_LESS;

	/* Render Properties for drawing texture */
	/* alpha test */
	rp_texture.mode.alphaTestMode.enabled = true;
	rp_texture.mode.alphaTestMode.alphafunc.func = R_GREATER;
	rp_texture.mode.alphaTestMode.alphafunc.ref = 0;

	/* lighting effect */
	rp_texture.mode.lightingMode.enabled = true;

	rp_texture.mode.normalizeMode.enabled = true;

	/* color blending */
	rp_texture.mode.blendMode.enabled = true;
	rp_texture.mode.blendMode.blendfunc.sfactor = R_SRC_ALPHA;
	rp_texture.mode.blendMode.blendfunc.dfactor = R_ONE_MINUS_SRC_ALPHA;
	
	/* depth buffer */
	rp_texture.mode.depthMaskMode.enabled = false;

	/* depth test */
	rp_texture.mode.depthTestMode.enabled = true;
	rp_texture.mode.depthTestMode.depthfunc.func = R_EQUAL;

	/* texture 2d */
	rp_texture.mode.texture2DMode.enabled = true;

	/* set texture parameter names and parameters */
	rp_texture.mode.texture2DMode.texParameteri.pname.clear();
	rp_texture.mode.texture2DMode.texParameteri.param.clear();

	rp_texture.mode.texture2DMode.texParameteri.pname.push_back( R_TEXTURE_WRAP_S );
	rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );

	rp_texture.mode.texture2DMode.texParameteri.pname.push_back( R_TEXTURE_WRAP_T );
	rp_texture.mode.texture2DMode.texParameteri.param.push_back( R_CLAMP_TO_EDGE );

	/* cull face */
	rp_texture.mode.cullFaceMode.enabled = true;
	rp_texture.mode.cullFaceMode.cullface.mode = R_BACK;
}