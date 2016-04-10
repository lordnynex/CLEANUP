#ifndef CVKALMAN_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvMat.h"

#define CVKALMAN_NAME "CvKalman"
#define checkCvKalman(L,i) luacv_checkObject<CvKalman>(L,i,CVKALMAN_NAME)
#define pushCvKalman(L,data) luacv_pushObject<CvKalman>(L,data,CVKALMAN_NAME,false)

extern const struct luaL_Reg CvKalman_m[];
#endif
