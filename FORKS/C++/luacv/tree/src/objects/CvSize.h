#ifndef CVSIZE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSIZE_NAME "CvSize"
#define checkCvSize(L,i) luacv_checkObject<CvSize>(L,i,CVSIZE_NAME)
#define pushCvSize(L,data) luacv_pushObject<CvSize>(L,data,CVSIZE_NAME,true)


extern const struct luaL_Reg CvSize_m[];

#endif
