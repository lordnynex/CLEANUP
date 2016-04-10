#ifndef CVHAIN_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeqBlock.h"
#include "CvMemStorage.h"
#include "CvPoint.h"
#include "CvSeq.h"

#define CVCHAIN_NAME "CvChain"

#define checkCvChain(L,i) luacv_checkObject<CvChain>(L,i,CVCHAIN_NAME)
#define pushCvChain(L,data) luacv_pushObject<CvChain>(L,data,CVCHAIN_NAME,false)

extern const struct luaL_Reg CvChain_m[];
#endif
