#ifndef CVSTARDETECTORPARAMS_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSTARDETECTORPARAMS_NAME "CvStarDetectorParams"
#define checkCvStarDetectorParams(L,i) luacv_checkObject<CvStarDetectorParams>(L,i,CVSTARDETECTORPARAMS_NAME)
#define pushCvStarDetectorParams(L,data) luacv_pushObject<CvStarDetectorParams>(L,data,CVSTARDETECTORPARAMS_NAME,true)

extern const struct luaL_Reg CvStarDetectorParams_m[];
#endif
