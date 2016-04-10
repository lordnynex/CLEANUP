#ifndef CVSTEREOBMSTATE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvMat.h"
#include "CvRect.h"

#define CVSTEREOBMSTATE_NAME "CvStereoBMState"
#define checkCvStereoBMState(L,i) luacv_checkObject<CvStereoBMState>(L,i,CVSTEREOBMSTATE_NAME)
#define pushCvStereoBMState(L,data) luacv_pushObject<CvStereoBMState>(L,data,CVSTEREOBMSTATE_NAME,false)

extern const struct luaL_Reg CvStereoBMState_m[];
#endif
