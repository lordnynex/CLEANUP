#ifndef LUACV_HIGHGUI

#define LUACV_HIGHGUI
#include "opencv2/opencv.hpp"
#include "luacvaux.h"

#include "lua_core.h"

#include "CvCapture.h"
#include "CvVideoWriter.h"

#define highguiReg()\
  objectReg(CVCAPTURE_NAME,CvCapture_m);\
  objectReg(CVVIDEOWRITER_NAME,CvVideoWriter_m);

extern luacv_callback *callbackTable[MAX_CALLBACKS];
extern const luaL_Reg highgui[];
extern const luacv_var highgui_var[];
#endif
