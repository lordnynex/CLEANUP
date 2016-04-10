#ifndef CVSIZE2D32F_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSIZE2D32F_NAME "CvSize2D32f"

#define checkCvSize2D32f(L,i) luacv_checkObject<CvSize2D32f>(L,i,CVSIZE2D32F_NAME)
#define pushCvSize2D32f(L,data) luacv_pushObject<CvSize2D32f>(L,data,CVSIZE2D32F_NAME,true)

extern const struct luaL_Reg CvSize2D32f_m[];
#endif
