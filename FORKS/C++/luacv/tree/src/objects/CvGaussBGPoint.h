#ifndef CVGAUSSBGPOINT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvGaussBGValues.h"

#define CVGAUSSBGPOINT_NAME "CvGaussBGPoint"

#define checkCvGaussBGPoint(L,i) luacv_checkObject<CvGaussBGPoint>(L,i,CVGAUSSBGPOINT_NAME)
#define pushCvGaussBGPoint(L,data) luacv_pushObject<CvGaussBGPoint>(L,data,CVGAUSSBGPOINT_NAME,true)

extern const struct luaL_Reg CvGaussBGPoint_m[];
#endif
