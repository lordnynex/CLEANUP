#ifndef CVSTARKEYPOINT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvPoint.h"

#define CVSTARKEYPOINT_NAME "CvStarKeypoint"
#define checkCvStarKeypoint(L,i) luacv_checkObject<CvStarKeypoint>(L,i,CVSTARKEYPOINT_NAME)
#define pushCvStarKeypoint(L,data) luacv_pushObject<CvStarKeypoint>(L,data,CVSTARKEYPOINT_NAME,true)

extern const struct luaL_Reg CvStarKeypoint_m[];
#endif
