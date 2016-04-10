#ifndef CVSUBDIV2DPOINT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvPoint2D32f.h"

#define CVSUBDIV2DPOINT_NAME "CvSubdiv2DPoint"
#define checkCvSubdiv2DPoint(L,i) luacv_checkObject<CvSubdiv2DPoint>(L,i,CVSUBDIV2DPOINT_NAME)
#define pushCvSubdiv2DPoint(L,data) luacv_pushObject<CvSubdiv2DPoint>(L,data,CVSUBDIV2DPOINT_NAME,true)

extern const struct luaL_Reg CvSubdiv2DPoint_m[];
#endif
