#ifndef CVOBJECTDETECTION_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "lua.h"
#include "CvRect.h"

#define CVOBJECTDETECTION_NAME "CvObjectDetection"

#define checkCvObjectDetection(L,i) luacv_checkObject<CvObjectDetection>(L,i,CVOBJECTDETECTION_NAME)
#define pushCvObjectDetection(L,data) luacv_pushObject<CvObjectDetection>(L,data,CVOBJECTDETECTION_NAME,true)

extern const struct luaL_Reg CvObjectDetection_m[];

#endif
