#ifndef CVCONTOUR_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvMemStorage.h"
#include "CvSeq.h"
#include "CvSetElem.h"
#include "CvPoint.h"
#include "CvRect.h"

#define CVCONTOUR_NAME "CvContour"

#define checkCvContour(L,i) luacv_checkObject<CvContour>(L,i,CVCONTOUR_NAME)
#define pushCvContour(L,data) luacv_pushObject<CvContour>(L,data,CVCONTOUR_NAME,false)

extern const struct luaL_Reg CvContour_m[];
#endif
