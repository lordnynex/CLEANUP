#ifndef  CVSETELEM_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSETELEM_NAME "CvSetElem"

#define checkCvSetElem(L,i) luacv_checkObject<CvSetElem>(L,i,CVSETELEM_NAME)
#define pushCvSetElem(L,data) luacv_pushObject<CvSetElem>(L,data,CVSETELEM_NAME,false)

extern const struct luaL_Reg CvSetElem_m[];
#endif
