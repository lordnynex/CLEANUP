#ifndef  CVSEQWRITER_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeq.h"
#include "CvSeqBlock.h"

#define CVSEQWRITER_NAME "CvSeqWriter"
#define checkCvSeqWriter(L,i) luacv_checkObject<CvSeqWriter>(L,i,CVSEQWRITER_NAME)
#define pushCvSeqWriter(L,data) luacv_pushObject<CvSeqWriter>(L,data,CVSEQWRITER_NAME,false)

extern const struct luaL_Reg CvSeqWriter_m[];
#endif
