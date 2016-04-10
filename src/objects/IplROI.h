#ifndef IPLROI_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define IPLROI_NAME "IplROI"
#define checkIplROI(L,i) luacv_checkObject<IplROI>(L,i,IPLROI_NAME)
#define pushIplROI(L,data) luacv_pushObject<IplROI>(L,data,IPLROI_NAME,true)

extern const struct luaL_Reg IplROI_m[];
#endif
