#ifndef  CVMEMSTORAGEPOS_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvMemBlock.h"

#define CVMEMSTORAGEPOS_NAME "CvMemStoragePos"

#define checkCvMemStoragePos(L,i) luacv_checkObject<CvMemStoragePos>(L,i,CVMEMSTORAGEPOS_NAME)
#define pushCvMemStoragePos(L,data) luacv_pushObject<CvMemStoragePos>(L,data,CVMEMSTORAGEPOS_NAME,false)

extern const struct luaL_Reg CvMemStoragePos_m[];
#endif
