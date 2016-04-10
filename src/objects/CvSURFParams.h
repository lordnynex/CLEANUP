#ifndef CVSURFPARAMS_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSURFPARAMS_NAME "CvSURFParams"

#define checkCvSURFParams(L,i) luacv_checkObject<CvSURFParams>(L,i,CVSURFPARAMS_NAME)
#define pushCvSURFParams(L,data) luacv_pushObject<CvSURFParams>(L,data,CVSURFPARAMS_NAME,true)

extern const struct luaL_Reg CvSURFParams_m[];

#endif
