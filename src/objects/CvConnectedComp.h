#ifndef CVCONNECTEDCOMP_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvScalar.h"
#include "CvRect.h"
#include "CvSeq.h"

#define CVCONNECTEDCOMP_NAME "CvConnectedComp"

#define checkCvConnectedComp(L,i) luacv_checkObject<CvConnectedComp>(L,i,CVCONNECTEDCOMP_NAME)
#define pushCvConnectedComp(L,data) luacv_pushObject<CvConnectedComp>(L,data,CVCONNECTEDCOMP_NAME,true)

extern const struct luaL_Reg CvConnectedComp_m[];
#endif
