#ifndef CVFONT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVFONT_NAME "CvFont"

#define checkCvFont(L,i) luacv_checkObject<CvFont>(L,i,CVFONT_NAME)
#define pushCvFont(L,data) luacv_pushObject<CvFont>(L,data,CVFONT_NAME,true)

extern const struct luaL_Reg CvFont_m[];
#endif
