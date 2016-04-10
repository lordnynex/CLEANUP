#ifndef  CVNARRAYITERATOR_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSize.h"
#include "CvMatND.h"

#define CVNARRAYITERATOR_NAME "CvNArrayIterator"
#define checkCvNArrayIterator(L,i) luacv_checkObject<CvNArrayIterator>(L,i,CVNARRAYITERATOR_NAME)
#define pushCvNArrayIterator(L,data) luacv_pushObject<CvNArrayIterator>(L,data,CVNARRAYITERATOR_NAME,false)

extern const struct luaL_Reg CvNArrayIterator_m[];
#endif
