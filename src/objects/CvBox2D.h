#ifndef CVBOX2D_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "lua.h"
#include "CvPoint2D32f.h"
#include "CvSize2D32f.h"

#define CVBOX2D_NAME "CvBox2D"

#define checkCvBox2D(L,i) luacv_checkObject<CvBox2D>(L,i,CVBOX2D_NAME)
#define pushCvBox2D(L,data) luacv_pushObject<CvBox2D>(L,data,CVBOX2D_NAME,true)

extern const struct luaL_Reg CvBox2D_m[];

#endif
