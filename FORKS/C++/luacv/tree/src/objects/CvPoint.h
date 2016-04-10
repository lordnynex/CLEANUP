#ifndef CVPOINT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVPOINT_NAME "CvPoint"
#define checkCvPoint(L,i) luacv_checkObject<CvPoint>(L,i,CVPOINT_NAME)
#define pushCvPoint(L,data) luacv_pushObject<CvPoint>(L,data,CVPOINT_NAME,true)

extern const struct luaL_Reg CvPoint_m[];
#endif
