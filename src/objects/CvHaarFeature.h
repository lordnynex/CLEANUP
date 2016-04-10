#ifndef CVHAARFEATURE_NAME

#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvRect.h"

#define CVHAARFEATURE_NAME "CvHaarFeature"

#define checkCvHaarFeature(L,i) luacv_checkObject<CvHaarFeature>(L,i,CVHAARFEATURE_NAME)
#define pushCvHaarFeature(L,data) luacv_pushObject<CvHaarFeature>(L,data,CVHAARFEATURE_NAME,false)


extern const struct luaL_Reg CvHaarFeature_m[];
#endif
