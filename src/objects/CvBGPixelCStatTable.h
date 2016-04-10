#ifndef CVBGPIXELCSTATTABLE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVBGPIXELCSTATTABLE_NAME "CvBGPixelCStatTable"

#define checkCvBGPixelCStatTable(L,i) luacv_checkObject<CvBGPixelCStatTable>(L,i,CVBGPIXELCSTATTABLE_NAME)
#define pushCvBGPixelCStatTable(L,data) luacv_pushObject<CvBGPixelCStatTable>(L,data,CVBGPIXELCSTATTABLE_NAME,true)

extern const struct luaL_Reg CvBGPixelCStatTable_m[];
#endif
