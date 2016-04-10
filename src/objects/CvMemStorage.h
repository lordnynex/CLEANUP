#ifndef CVMEMSTORAGE_NAME 

#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvMemBlock.h"

#define CVMEMSTORAGE_NAME "CvMemStorage"

#define checkCvMemStorage(L,i) luacv_checkObject<CvMemStorage>(L,i,CVMEMSTORAGE_NAME)
#define pushCvMemStorage(L,data) luacv_pushObject<CvMemStorage>(L,data,CVMEMSTORAGE_NAME,false)

extern const struct luaL_Reg CvMemStorage_m[];
#endif
