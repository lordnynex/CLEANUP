#ifndef CVFGDSTATMODELPARAMS_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVFGDSTATMODELPARAMS_NAME "CvFGDStatModelParams"
#define checkCvFGDStatModelParams(L,i) luacv_checkObject<CvFGDStatModelParams>(L,i,CVFGDSTATMODELPARAMS_NAME)
#define pushCvFGDStatModelParams(L,data) luacv_pushObject<CvFGDStatModelParams>(L,data,CVFGDSTATMODELPARAMS_NAME,true)

extern const struct luaL_Reg CvFGDStatModelParams_m[];
#endif
