#ifdef _MSC_VER
#	pragma comment(lib, "opengl32.lib")
#endif

#ifdef _WIN32
#	include <windows.h>
#endif

#ifdef __APPLE__
#	include <OpenGL\gl.h>
#else
#	include <gl\GL.h>
#	include "glext.h"
#endif

#include <assert.h>

#include "GLVSPFont.h"

GLVSPFont::GLVSPFont()
{
}
GLVSPFont::~GLVSPFont()
{
	for (int i = 0; i < (int)fontVec.size(); i++)
	{
		delete fontVec[i].second;
	}
}

int GLVSPFont::loadFont( const char * file )
{
	// get font
	for (int i = 0; i < (int)fontVec.size(); i++)
	{
		pair< Stringc, GLFont * > fontPair = fontVec[i];

		if ((char*)fontPair.first == file)
		{
			font = fontPair.second;
			return 1;
		}
	}

	// create font
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLFont * texfont = new GLFont();
	GLuint texID;
	glGenTextures(1, &texID);
	if ( !texfont->Create(file, texID) )
	{
		delete texfont;
		texfont = 0;

		font = texfont;
		return 0;
	}
	else
	{
		fontVec.push_back( pair< Stringc, GLFont * >(Stringc(file), texfont) );
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		font = texfont;
		return 1;
	}
}

void GLVSPFont::draw( Stringc str, float scale, float x0, float y0, float xoffset, float yoffset )
{
	if ( !font )
		return;

	glEnable( GL_TEXTURE_2D );

	double w = 0; 
	double h = 0;
	if (str.get_length() > 0) 
	{
		//alignment 
		pair< int, int > dimension;
		font->GetStringSize(str.get_char_star(), &dimension);
		w = ((float) dimension.first) * FONT_BASE_SCALE * scale;		
		h = ((float) dimension.second) * FONT_BASE_SCALE * scale;
		double x = x0 - w/2  + w*xoffset;
		double y = y0 + h/2 + h*yoffset;

		font->Begin();
		font->DrawString(str.get_char_star(), (float)(FONT_BASE_SCALE * scale), (float)x, (float)y);
	}

	glDisable( GL_TEXTURE_2D );
}

void GLVSPFont::draw( Stringc str, float scale, float * trans_mat, float x0, float y0, float xoffset, float yoffset )
{
	glPushMatrix();
	glMultMatrixf( (GLfloat*)trans_mat );

	draw( str, scale, x0, y0, xoffset, yoffset );

	glPopMatrix();
}