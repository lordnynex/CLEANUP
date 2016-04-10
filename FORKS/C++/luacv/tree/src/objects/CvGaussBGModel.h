#ifndef CVGAUSSBGMODEL_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "lua_core.h"
#include "CvGaussBGStatModelParams.h"
#include "CvGaussBGValues.h"
#include "CvGaussBGPoint.h"

#define CVGAUSSBGMODEL_NAME "CvGaussBGModel"
#define checkCvGaussBGModel(L,i) luacv_checkObject<CvGaussBGModel>(L,i,CVGAUSSBGMODEL_NAME)
#define pushCvGaussBGModel(L,data) luacv_pushObject<CvGaussBGModel>(L,data,CVGAUSSBGMODEL_NAME,false)

extern const struct luaL_Reg CvGaussBGModel_m[];

#endif
