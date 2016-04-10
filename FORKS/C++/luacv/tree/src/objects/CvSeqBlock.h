#ifndef  CVSEQBLOCK_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSEQBLOCK_NAME "CvSeqBlock"
#define checkCvSeqBlock(L,i) luacv_checkObject<CvSeqBlock>(L,i,CVSEQBLOCK_NAME)
#define pushCvSeqBlock(L,data) luacv_pushObject<CvSeqBlock>(L,data,CVSEQBLOCK_NAME,false)

extern const struct luaL_Reg CvSeqBlock_m[];
#endif
