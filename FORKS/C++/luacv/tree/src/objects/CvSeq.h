#ifndef CVSEQ_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeqBlock.h"
#include "CvMemStorage.h"

#define CVSEQ_NAME "CvSeq"

#define checkCvSeq(L,i) luacv_checkObject<CvSeq>(L,i,CVSEQ_NAME)
#define pushCvSeq(L,data) luacv_pushObject<CvSeq>(L,data,CVSEQ_NAME,false)

extern const struct luaL_Reg CvSeq_m[];
#endif
