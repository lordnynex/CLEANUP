#ifndef CVGAUSSBGVALUES_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVGAUSSBGVALUES_NAME "CvGaussBGValues"

#define checkCvGaussBGValues(L,i) luacv_checkObject<CvGaussBGValues>(L,i,CVGAUSSBGVALUES_NAME)
#define pushCvGaussBGValues(L,data) luacv_pushObject<CvGaussBGValues>(L,data,CVGAUSSBGVALUES_NAME,true)

extern const struct luaL_Reg CvGaussBGValues_m[];
#endif
