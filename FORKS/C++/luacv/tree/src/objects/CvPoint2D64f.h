#ifndef CVPOINT2D64F_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVPOINT2D64F_NAME "CvPoint2D64f"
#define checkCvPoint2D64f(L,i) luacv_checkObject<CvPoint2D64f>(L,i,CVPOINT2D64F_NAME)
#define pushCvPoint2D64f(L,data) luacv_pushObject<CvPoint2D64f>(L,data,CVPOINT2D64F_NAME,true)

extern const struct luaL_Reg CvPoint2D64f_m[];
#endif
