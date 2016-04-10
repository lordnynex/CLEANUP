#ifndef CVLSVMFILTERPOSITION_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVLSVMFILTERPOSITION_NAME "CvLSVMFilterPosition"
#define checkCvLSVMFilterPosition(L,i) luacv_checkObject<CvLSVMFilterPosition>(L,i,CVLSVMFILTERPOSITION_NAME)
#define pushCvLSVMFilterPosition(L,data) luacv_pushObject<CvLSVMFilterPosition>(L,data,CVLSVMFILTERPOSITION_NAME,true)

extern const struct luaL_Reg CvLSVMFilterPosition_m[];
#endif
