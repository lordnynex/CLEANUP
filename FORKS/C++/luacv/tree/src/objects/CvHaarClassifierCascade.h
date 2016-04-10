#ifndef CVHAARCLASSIFIERCASCADE_NAME

#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSize.h"
#include "CvHaarStageClassifier.h"

#define CVHAARCLASSIFIERCASCADE_NAME "CvHaarClassifierCascade"
#define checkCvHaarClassifierCascade(L,i) luacv_checkObject<CvHaarClassifierCascade>(L,i,CVHAARCLASSIFIERCASCADE_NAME)
#define pushCvHaarClassifierCascade(L,data) luacv_pushObject<CvHaarClassifierCascade>(L,data,CVHAARCLASSIFIERCASCADE_NAME,false)

extern const struct luaL_Reg CvHaarClassifierCascade_m[];
#endif
