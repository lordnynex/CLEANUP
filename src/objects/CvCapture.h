#ifndef CVCAPTURE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVCAPTURE_NAME "CvCapture"

#define checkCvCapture(L,i) luacv_checkObject<CvCapture>(L,i,CVCAPTURE_NAME)
void pushCvCapture(lua_State *L,CvCapture *);


extern const struct luaL_Reg CvCapture_m[];
#endif
