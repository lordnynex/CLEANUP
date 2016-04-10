#ifndef CVBGCODEBOOKELEM_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVBGCODEBOOKELEM_NAME "CvBGCodeBookElem"

#define checkCvBGCodeBookElem(L,i) luacv_checkObject<CvBGCodeBookElem>(L,i,CVBGCODEBOOKELEM_NAME)
#define pushCvBGCodeBookElem(L,data) luacv_pushObject<CvBGCodeBookElem>(L,data,CVBGCODEBOOKELEM_NAME,true)

extern const struct luaL_Reg CvBGCodeBookElem_m[];
#endif
