#ifndef CVGRAPHVTX2D_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvGraphEdge.h"
#include "CvPoint2D32f.h"
#include "CvGraphVtx.h"

#define CVGRAPHVTX2D_NAME "CvGraphVtx2D"

#define checkCvGraphVtx2D(L,i) luacv_checkObject<CvGraphVtx2D>(L,i,CVGRAPHVTX2D_NAME)
#define pushCvGraphVtx2D(L,data) luacv_pushObject<CvGraphVtx2D>(L,data,CVGRAPHVTX2D_NAME,false)

extern const struct luaL_Reg CvGraphVtx2D_m[];
#endif
