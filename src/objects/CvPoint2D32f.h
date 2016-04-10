#ifndef CVPOINT2D32F_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVPOINT2D32F_NAME "CvPoint2D32f"
#define checkCvPoint2D32f(L,i) luacv_checkObject<CvPoint2D32f>(L,i,CVPOINT2D32F_NAME)
#define pushCvPoint2D32f(L,data) luacv_pushObject<CvPoint2D32f>(L,data,CVPOINT2D32F_NAME,true)

extern const struct luaL_Reg CvPoint2D32f_m[];
#endif
