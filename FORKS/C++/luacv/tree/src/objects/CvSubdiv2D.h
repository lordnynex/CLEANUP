#ifndef CVSUBDIV2D_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvGraph.h"
#include "CvPoint2D32f.h"

#define CVSUBDIV2D_NAME "CvSubdiv2D"

#define checkCvSubdiv2D(L,i) luacv_checkObject<CvSubdiv2D>(L,i,CVSUBDIV2D_NAME)
#define pushCvSubdiv2D(L,data) luacv_pushObject<CvSubdiv2D>(L,data,CVSUBDIV2D_NAME,false)

extern const struct luaL_Reg CvSubdiv2D_m[];
#endif
