#ifndef CVSLICE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSLICE_NAME "CvSlice"

#define checkCvSlice(L,i) luacv_checkObject<CvSlice>(L,i,CVSLICE_NAME)
#define pushCvSlice(L,data) luacv_pushObject<CvSlice>(L,data,CVSLICE_NAME,true)

extern const struct luaL_Reg CvSlice_m[];
#endif
