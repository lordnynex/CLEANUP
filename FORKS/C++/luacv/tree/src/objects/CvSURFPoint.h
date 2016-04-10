#ifndef CVSURFPOINT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvPoint2D32f.h"

#define CVSURFPOINT_NAME "CvSURFPoint"

#define checkCvSURFPoint(L,i) luacv_checkObject<CvSURFPoint>(L,i,CVSURFPOINT_NAME)
#define pushCvSURFPoint(L,data) luacv_pushObject<CvSURFPoint>(L,data,CVSURFPOINT_NAME,true)

extern const struct luaL_Reg CvSURFPoint_m[];

#endif
