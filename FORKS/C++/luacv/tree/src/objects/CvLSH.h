#ifndef CVLSH_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVLSH_NAME "CvLSH"

#define checkCvLSH(L,i) luacv_checkObject<CvLSH>(L,i,CVLSH_NAME)
void pushCvLSH(lua_State *L,CvLSH *);

extern const struct luaL_Reg CvLSH_m[];
#endif
