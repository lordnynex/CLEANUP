#ifndef CVGAUSSBGSTATMODELPARAMS_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVGAUSSBGSTATMODELPARAMS_NAME "CvGaussBGStatModelParams"
#define checkCvGaussBGStatModelParams(L,i) luacv_checkObject<CvGaussBGStatModelParams>(L,i,CVGAUSSBGSTATMODELPARAMS_NAME)
#define pushCvGaussBGStatModelParams(L,data) luacv_pushObject<CvGaussBGStatModelParams>(L,data,CVGAUSSBGSTATMODELPARAMS_NAME,true)

extern const struct luaL_Reg CvGaussBGStatModelParams_m[];
#endif
