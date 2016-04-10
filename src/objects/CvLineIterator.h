#ifndef  CVLINEITERATOR_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVLINEITERATOR_NAME "CvLineIterator"
#define checkCvLineIterator(L,i) luacv_checkObject<CvLineIterator>(L,i,CVLINEITERATOR_NAME)
#define pushCvLineIterator(L,data) luacv_pushObject<CvLineIterator>(L,data,CVLINEITERATOR_NAME,true)

extern const struct luaL_Reg CvLineIterator_m[];
#endif
