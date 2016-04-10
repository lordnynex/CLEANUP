#ifndef LUACV_FEATURES2D

#define LUACV_FEATURES2D
#include "opencv2/opencv.hpp"
#include "luacvaux.h"
#include "lua_core.h"

#include "CvSURFPoint.h"
#include "CvSURFParams.h"
#include "CvMSERParams.h"
#include "CvStarKeypoint.h"
#include "CvStarDetectorParams.h"

#define features2dReg()\
  objectReg(CVSURFPOINT_NAME,CvSURFPoint_m);\
  objectReg(CVSURFPARAMS_NAME,CvSURFParams_m);\
  objectReg(CVMSERPARAMS_NAME,CvMSERParams_m);\
  objectReg(CVSTARKEYPOINT_NAME,CvStarKeypoint_m);\
  objectReg(CVSTARDETECTORPARAMS_NAME,CvStarDetectorParams_m);

extern const luaL_Reg features2d[];
extern const luacv_var features2d_var[];
extern const luacv_var features2d_object[];

#endif
