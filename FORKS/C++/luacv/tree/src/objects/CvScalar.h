#ifndef CVSCALAR_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSCALAR_NAME "CvScalar"

#define checkCvScalar(L,i) luacv_checkObject<CvScalar>(L,i,CVSCALAR_NAME)
#define pushCvScalar(L,data) luacv_pushObject<CvScalar>(L,data,CVSCALAR_NAME,true)

extern const struct luaL_Reg CvScalar_m[];
#endif
