#ifndef CVSUBDIV2DEDGE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSUBDIV2DEDGE_NAME "CvSubdiv2DEdge"

#define checkCvSubdiv2DEdge(L,i) luacv_checkObject<CvSubdiv2DEdge>(L,i,CVSUBDIV2DEDGE_NAME)

#define pushCvSubdiv2DEdge(L,edge) luacv_pushObject(L,edge,CVSUBDIV2DEDGE_NAME,true)

extern const struct luaL_Reg CvSubdiv2DEdge_m[];
#endif
