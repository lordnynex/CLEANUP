#ifndef  CVSPARSEMAT_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSet.h"

#define CVSPARSEMAT_NAME "CvSparseMat"
#define checkCvSparseMat(L,i) luacv_checkObject<CvSparseMat>(L,i,CVSPARSEMAT_NAME)
#define pushCvSparseMat(L,data) luacv_pushObject<CvSparseMat>(L,data,CVSPARSEMAT_NAME,false)

extern const struct luaL_Reg CvSparseMat_m[];
#endif
