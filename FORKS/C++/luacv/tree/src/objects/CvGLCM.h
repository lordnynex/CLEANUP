#ifndef CVGLCM_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVGLCM_NAME "CvGLCM"

#define checkCvGLCM(L,i) luacv_checkObject<CvGLCM>(L,i,CVGLCM_NAME)
void pushCvGLCM(lua_State *L,CvGLCM *);

extern const struct luaL_Reg CvGLCM_m[];
#endif
