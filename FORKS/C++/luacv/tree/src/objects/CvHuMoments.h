#ifndef CVHUMOMENTS_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVHUMOMENTS_NAME "CvHuMoments"
#define checkCvHuMoments(L,i) luacv_checkObject<CvHuMoments>(L,i,CVHUMOMENTS_NAME)
#define pushCvHuMoments(L,data) luacv_pushObject<CvHuMoments>(L,data,CVHUMOMENTS_NAME,true)

extern const struct luaL_Reg CvHuMoments_m[];
#endif
