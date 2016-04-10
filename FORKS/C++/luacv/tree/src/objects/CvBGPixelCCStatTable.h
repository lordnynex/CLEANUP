#ifndef CVBGPIXELCCSTATTABLE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVBGPIXELCCSTATTABLE_NAME "CvBGPixelCCStatTable"

#define checkCvBGPixelCCStatTable(L,i) luacv_checkObject<CvBGPixelCCStatTable>(L,i,CVBGPIXELCCSTATTABLE_NAME)
#define pushCvBGPixelCCStatTable(L,data) luacv_pushObject<CvBGPixelCCStatTable>(L,data,CVBGPIXELCCSTATTABLE_NAME,true)

extern const struct luaL_Reg CvBGPixelCCStatTable_m[];
#endif
