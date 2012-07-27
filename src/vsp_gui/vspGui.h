//****************************************************************************
//    
// vspGui.h: API for the VSP gui interface
//
// Phoenix Integration - 5/15/12
//
//****************************************************************************

#ifndef _VSP_GUI_H_
#define _VSP_GUI_H_

#ifdef _WIN32
#	include <Windows.h>
#endif

#include <FL/Fl.H>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif


/******************************************************
*
* VSP Gui class.
*
*******************************************************/
class vspGui
{
public:
	static int run();

private:
	Fl_Widget *w;
};

#endif //_VSP_GUI_H