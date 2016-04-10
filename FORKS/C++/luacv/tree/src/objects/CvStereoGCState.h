#ifndef CVSTEREOGCSTATE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvMat.h"

#define CVSTEREOGCSTATE_NAME "CvStereoGCState"
#define checkCvStereoGCState(L,i) luacv_checkObject<CvStereoGCState>(L,i,CVSTEREOGCSTATE_NAME)
#define pushCvStereoGCState(L,data) luacv_pushObject<CvStereoGCState>(L,data,CVSTEREOGCSTATE_NAME,false)

extern const struct luaL_Reg CvStereoGCState_m[];
#endif
