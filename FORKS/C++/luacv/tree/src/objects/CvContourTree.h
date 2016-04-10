#ifndef CVCONTOURTREE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "CvSeq.h"
#include "CvPoint.h"

#define CVCONTOURTREE_NAME "CvContourTree"

#define checkCvContourTree(L,i) luacv_checkObject<CvContourTree>(L,i,CVCONTOURTREE_NAME)
#define pushCvContourTree(L,data) luacv_pushObject<CvContourTree>(L,data,CVCONTOURTREE_NAME,false)

extern const struct luaL_Reg CvContourTree_m[];
#endif
