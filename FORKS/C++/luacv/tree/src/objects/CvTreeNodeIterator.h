#ifndef CVTREENODEITERATOR_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVTREENODEITERATOR_NAME "CvTreeNodeIterator"

#define checkCvTreeNodeIterator(L,i) luacv_checkObject<CvTreeNodeIterator>(L,i,CVTREENODEITERATOR_NAME)
#define pushCvTreeNodeIterator(L,data) luacv_pushObject<CvTreeNodeIterator>(L,data,CVTREENODEITERATOR_NAME,true)

extern const struct luaL_Reg CvTreeNodeIterator_m[];
#endif
