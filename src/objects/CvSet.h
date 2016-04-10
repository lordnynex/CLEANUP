#ifndef CVSET_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeq.h"
#include "CvSetElem.h"

#define CVSET_NAME "CvSet"
#define checkCvSet(L,i) luacv_checkObject<CvSet>(L,i,CVSET_NAME)
#define pushCvSet(L,data) luacv_pushObject<CvSet>(L,data,CVSET_NAME,false)

extern const struct luaL_Reg CvSet_m[];
#endif
