#ifndef  CVSEQREADER_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeq.h"
#include "CvSeqBlock.h"

#define CVSEQREADER_NAME "CvSeqReader"

#define checkCvSeqReader(L,i) luacv_checkObject<CvSeqReader>(L,i,CVSEQREADER_NAME)
#define pushCvSeqReader(L,data) luacv_pushObject<CvSeqReader>(L,data,CVSEQREADER_NAME,true)

extern const struct luaL_Reg CvSeqReader_m[];
#endif
