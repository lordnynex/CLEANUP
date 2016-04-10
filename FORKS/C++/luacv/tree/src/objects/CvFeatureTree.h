#ifndef CVFEATURETREE_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVFEATURETREE_NAME "CvFeatureTree"

#define checkCvFeatureTree(L,i) luacv_checkObject<CvFeatureTree>(L,i,CVFEATURETREE_NAME)
void pushCvFeatureTree(lua_State *L,CvFeatureTree *);


extern const struct luaL_Reg CvFeatureTree_m[];
#endif
