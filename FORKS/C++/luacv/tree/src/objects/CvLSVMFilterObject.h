#ifndef CVLSVMFILTEROBJECT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvLSVMFilterPosition.h"

#define CVLSVMFILTEROBJECT_NAME "CvLSVMFilterObject"

#define checkCvLSVMFilterObject(L,i) luacv_checkObject<CvLSVMFilterObject>(L,i,CVLSVMFILTEROBJECT_NAME)
#define pushCvLSVMFilterObject(L,data) luacv_pushObject<CvLSVMFilterObject>(L,data,CVLSVMFILTEROBJECT_NAME,true)

extern const struct luaL_Reg CvLSVMFilterObject_m[];
#endif
