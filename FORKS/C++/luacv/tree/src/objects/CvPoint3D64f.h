#ifndef CVPOINT3D64F_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVPOINT3D64F_NAME "CvPoint3D64f"
#define checkCvPoint3D64f(L,i) luacv_checkObject<CvPoint3D64f>(L,i,CVPOINT3D64F_NAME)
#define pushCvPoint3D64f(L,data) luacv_pushObject<CvPoint3D64f>(L,data,CVPOINT3D64F_NAME,true)

extern const struct luaL_Reg CvPoint3D64f_m[];
#endif
