#ifndef  CVSPARSENODE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVSPARSENODE_NAME "CvSparseNode"

#define checkCvSparseNode(L,i) luacv_checkObject<CvSparseNode>(L,i,CVSPARSENODE_NAME)
#define pushCvSparseNode(L,data) luacv_pushObject<CvSparseNode>(L,data,CVSPARSENODE_NAME,false)

extern const struct luaL_Reg CvSparseNode_m[];
#endif
