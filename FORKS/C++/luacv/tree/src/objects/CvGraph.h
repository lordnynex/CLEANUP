#ifndef CVGRAPH_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSet.h"
#include "CvMemStorage.h"
#include "CvSeq.h"
#include "CvSetElem.h"


#define CVGRAPH_NAME "CvGraph"

#define checkCvGraph(L,i) luacv_checkObject<CvGraph>(L,i,CVGRAPH_NAME)
#define pushCvGraph(L,data) luacv_pushObject<CvGraph>(L,data,CVGRAPH_NAME,false)

extern const struct luaL_Reg CvGraph_m[];
#endif
