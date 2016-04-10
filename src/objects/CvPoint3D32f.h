#ifndef CVPOINT3D32F_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVPOINT3D32F_NAME "CvPoint3D32f"
#define checkCvPoint3D32f(L,i) luacv_checkObject<CvPoint3D32f>(L,i,CVPOINT3D32F_NAME)
#define pushCvPoint3D32f(L,data) luacv_pushObject<CvPoint3D32f>(L,data,CVPOINT3D32F_NAME,true)

extern const struct luaL_Reg CvPoint3D32f_m[];
#endif
