#ifndef  CVCHAINPTREADER_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeqReader.h"
#include "CvPoint.h"

#define CVCHAINPTREADER_NAME "CvChainPtReader"

#define checkCvChainPtReader(L,i) luacv_checkObject<CvChainPtReader>(L,i,CVCHAINPTREADER_NAME)
#define pushCvChainPtReader(L,data) luacv_pushObject<CvChainPtReader>(L,data,CVCHAINPTREADER_NAME,false)

extern const struct luaL_Reg CvChainPtReader_m[];
#endif
