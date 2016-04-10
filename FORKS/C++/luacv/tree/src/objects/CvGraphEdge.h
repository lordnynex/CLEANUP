#ifndef CVGRAPHEDGE_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVGRAPHEDGE_NAME "CvGraphEdge"
#define checkCvGraphEdge(L,i) luacv_checkObject<CvGraphEdge>(L,i,CVGRAPHEDGE_NAME)
#define pushCvGraphEdge(L,data) luacv_pushObject<CvGraphEdge>(L,data,CVGRAPHEDGE_NAME,false)

extern const struct luaL_Reg CvGraphEdge_m[];
#endif
