#ifndef CVMOMENTS_NAME

#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVMOMENTS_NAME "CvMoments"

#define checkCvMoments(L,i) luacv_checkObject<CvMoments>(L,i,CVMOMENTS_NAME)
#define pushCvMoments(L,data) luacv_pushObject<CvMoments>(L,data,CVMOMENTS_NAME,true)

extern const struct luaL_Reg CvMoments_m[];
#endif
