#ifndef TEXTURE_LOADER_INTERFACE
#define TEXTURE_LOADER_INTERFACE

#include "stringc.h"

class ITextureLoader
{
public:
	ITextureLoader() {}
	virtual ~ITextureLoader() {}

public:
	int loadTex( const char* name ) { return 0; }
};

#endif