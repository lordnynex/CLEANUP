#ifndef CVRECT_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVRECT_NAME "CvRect"
#define checkCvRect(L,i) luacv_checkObject<CvRect>(L,i,CVRECT_NAME)
#define pushCvRect(L,data) luacv_pushObject<CvRect>(L,data,CVRECT_NAME,true)

extern const struct luaL_Reg CvRect_m[];
#endif
