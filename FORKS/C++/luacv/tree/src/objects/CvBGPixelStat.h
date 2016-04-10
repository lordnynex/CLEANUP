#ifndef CVBGPIXELSTAT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "lua.h"
#include "CvBGPixelCStatTable.h"
#include "CvBGPixelCCStatTable.h"


#define CVBGPIXELSTAT_NAME "CvBGPixelStat"

#define checkCvBGPixelStat(L,i) luacv_checkObject<CvBGPixelStat>(L,i,CVBGPIXELSTAT_NAME)
#define pushCvBGPixelStat(L,data) luacv_pushObject<CvBGPixelStat>(L,data,CVBGPIXELSTAT_NAME,true)

extern const struct luaL_Reg CvBGPixelStat_m[];

#endif
