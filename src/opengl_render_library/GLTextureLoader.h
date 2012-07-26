#ifndef OPENGL_TEXTURE_LOADER_H_1
#define OPENGL_TEXTURE_LOADER_H_1

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "xmlvsp.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>		
#endif

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/gl.h>
#  include "glext.h"
#endif

#include "stringc.h"
#include <string>
#include <vector>
#include <map>

#include "IRenderer.h"

#define JPG_EXT	".jpg"
#define JPEG_EXT	".jpeg"
#define TGA_EXT	".tga"
#define PNG_EXT	".png"

typedef struct
{
	GLuint		bytesPerPixel;			// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;				// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;					// Temporary Variable
	GLuint		type;	
} TGA;

typedef struct {
   char  idlength;			// size of string that follows header (bytes)
   char  colourmaptype;		// 0: BGR, 1: Paletted
   char  datatypecode;		// 1: uncompr Pal, 2: uncompr BGR, 9: compr Pal, 10: compr BGR
   char  lo_colourmaporigin;	// offset in palettespace (lo byte of short)
   char  hi_colourmaporigin;	// offset in palettespace (hi byte of short)
   char  lo_colourmaplength;	// number of entries in colour map (lo byte)
   char  hi_colourmaplength;	// number of entries in colour map (hi byte)
   char  colourmapdepth;	// size of entries in colour map (bits)
   short x_origin;			// offset in screenspace
   short y_origin;			// offset in screenspace
   short width;				// image width
   short height;			// image height
   char  bitsperpixel;		// size of image data entries (bits)
   char  imagedescriptor;	// packed bits
} TGAHeader;

typedef	struct									
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
} TexData;

class Texture
{
public:
	Texture();
	virtual ~Texture()		{}

	unsigned int getTexID()				{ return texID; }

	enum { TEX_TYPE_INVALID, TEX_TYPE_TGA, TEX_TYPE_JPG };

	Stringc name;

	GLuint width;
	GLuint height;
	GLuint texID;

	bool readFile( const char* fileName, int type );
	bool loadTGA(TexData* texture, const char* filename);		
};

class TextureLoader : public ITextureLoader
{
public:
	TextureLoader();
	virtual ~TextureLoader();

public:
	virtual int loadTex( const char* name );

private:
	map< string, Texture* >::iterator texMapIter;
	map< string, Texture* > texMap;
};
#endif