#ifndef CVHAARCLASSIFIER_NAME

#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvHaarFeature.h"

#define CVHAARCLASSIFIER_NAME "CvHaarClassifier"
#define checkCvHaarClassifier(L,i) luacv_checkObject<CvHaarClassifier>(L,i,CVHAARCLASSIFIER_NAME)
#define pushCvHaarClassifier(L,data) luacv_pushObject<CvHaarClassifier>(L,data,CVHAARCLASSIFIER_NAME,false)

extern const struct luaL_Reg CvHaarClassifier_m[];
#endif
