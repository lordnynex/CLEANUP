#ifndef CVMAT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVMAT_NAME "CvMat"
#define checkCvMat(L,i) luacv_checkObject<CvMat>(L,i,CVMAT_NAME)
#define pushCvMat(L,data) luacv_pushObject<CvMat>(L,data,CVMAT_NAME,false)

extern const struct luaL_Reg CvMat_m[];
#endif
