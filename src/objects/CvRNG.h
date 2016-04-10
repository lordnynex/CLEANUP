#ifndef CVRNG_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVRNG_NAME "CvRNG"
#define checkCvRNG(L,i) luacv_checkObject<CvRNG>(L,i,CVRNG_NAME)
#define pushCvRNG(L,data) luacv_pushObject<CvRNG>(L,data,CVRNG_NAME,true)

extern const struct luaL_Reg CvRNG_m[];
#endif
