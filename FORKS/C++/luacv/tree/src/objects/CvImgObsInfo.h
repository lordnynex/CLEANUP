#ifndef CVIMGOBSINFO_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"

#define CVIMGOBSINFO_NAME "CvImgObsInfo"
#define checkCvImgObsInfo(L,i) luacv_checkObject<CvImgObsInfo>(L,i,CVIMGOBSINFO_NAME)
#define pushCvImgObsInfo(L,data) luacv_pushObject<CvImgObsInfo>(L,data,CVIMGOBSINFO_NAME,false)

extern const struct luaL_Reg CvImgObsInfo_m[];
#endif
