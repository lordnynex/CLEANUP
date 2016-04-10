#ifndef CVGRAPHSCANNER_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvGraph.h"
#include "CvGraphVtx.h"

#define CVGRAPHSCANNER_NAME "CvGraphScanner"

#define checkCvGraphScanner(L,i) luacv_checkObject<CvGraphScanner>(L,i,CVGRAPHSCANNER_NAME)
#define pushCvGraphScanner(L,data) luacv_pushObject<CvGraphScanner>(L,data,CVGRAPHSCANNER_NAME,false)

extern const struct luaL_Reg CvGraphScanner_m[];
#endif
