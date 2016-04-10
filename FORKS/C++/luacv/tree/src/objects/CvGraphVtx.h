#ifndef CVGRAPHVTX_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvGraphEdge.h"

#define CVGRAPHVTX_NAME "CvGraphVtx"
#define checkCvGraphVtx(L,i) luacv_checkObject<CvGraphVtx>(L,i,CVGRAPHVTX_NAME)
#define pushCvGraphVtx(L,data) luacv_pushObject<CvGraphVtx>(L,data,CVGRAPHVTX_NAME,true)

extern const struct luaL_Reg CvGraphVtx_m[];
#endif
