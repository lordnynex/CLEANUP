#ifndef CVHAARSTAGECLASSIFIER_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvHaarClassifier.h"

#define CVHAARSTAGECLASSIFIER_NAME "CvHaarStageClassifier"

#define checkCvHaarStageClassifier(L,i) luacv_checkObject<CvHaarStageClassifier>(L,i,CVHAARSTAGECLASSIFIER_NAME)
#define pushCvHaarStageClassifier(L,data) luacv_pushObject<CvHaarStageClassifier>(L,data,CVHAARSTAGECLASSIFIER_NAME,true)

extern const struct luaL_Reg CvHaarStageClassifier_m[];
#endif
