#ifndef CVQUADEDGE2D_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSubdiv2DPoint.h"
#include "CvSubdiv2DEdge.h"

#define CVQUADEDGE2D_NAME "CvQuadEdge2D"
#define checkCvQuadEdge2D(L,i) luacv_checkObject<CvQuadEdge2D>(L,i,CVQUADEDGE2D_NAME)
#define pushCvQuadEdge2D(L,data) luacv_pushObject<CvQuadEdge2D>(L,data,CVQUADEDGE2D_NAME,true)

extern const struct luaL_Reg CvQuadEdge2D_m[];
#endif
