#ifndef CVHISTOGRAM_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvMatND.h"
#include "CvArr.h"

#define CVHISTOGRAM_NAME "CvHistogram"

#define checkCvHistogram(L,i) luacv_checkObject<CvHistogram>(L,i,CVHISTOGRAM_NAME)
#define pushCvHistogram(L,data) luacv_pushObject<CvHistogram>(L,data,CVHISTOGRAM_NAME,false)

extern const struct luaL_Reg CvHistogram_m[];
#endif
