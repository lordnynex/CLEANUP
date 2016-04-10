#ifndef  CVSPARSEMATITERATOR_NAME 
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvSparseMat.h"
#include "CvSparseNode.h"


#define CVSPARSEMATITERATOR_NAME "CvSparseMatIterator"
#define checkCvSparseMatIterator(L,i) luacv_checkObject<CvSparseMatIterator>(L,i,CVSPARSEMATITERATOR_NAME)
#define pushCvSparseMatIterator(L,data) luacv_pushObject<CvSparseMatIterator>(L,data,CVSPARSEMATITERATOR_NAME,true)

extern const struct luaL_Reg CvSparseMatIterator_m[];
#endif
