#ifndef CVMEMBLOCK_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVMEMBLOCK_NAME "CvMemBlock"

#define checkCvMemBlock(L,i) luacv_checkObject<CvMemBlock>(L,i,CVMEMBLOCK_NAME)
#define pushCvMemBlock(L,data) luacv_pushObject<CvMemBlock>(L,data,CVMEMBLOCK_NAME,true)

extern const struct luaL_Reg CvMemBlock_m[];
#endif
