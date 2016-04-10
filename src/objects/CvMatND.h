#ifndef  CVMATND_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVMATND_NAME "CvMatND"

#define checkCvMatND(L,i) luacv_checkObject<CvMatND>(L,i,CVMATND_NAME)
#define pushCvMatND(L,data) luacv_pushObject<CvMatND>(L,data,CVMATND_NAME,false)

extern const struct luaL_Reg CvMatND_m[];
#endif
