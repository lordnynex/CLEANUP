#ifndef CVMSERPARAMS_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVMSERPARAMS_NAME "CvMSERParams"

#define checkCvMSERParams(L,i) luacv_checkObject<CvMSERParams>(L,i,CVMSERPARAMS_NAME)
#define pushCvMSERParams(L,data) luacv_pushObject<CvMSERParams>(L,data,CVMSERPARAMS_NAME,true)

extern const struct luaL_Reg CvMSERParams_m[];
#endif
