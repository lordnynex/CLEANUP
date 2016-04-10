#ifndef CVFGDSTATMODEL_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "lua_core.h"

#include "CvBGPixelStat.h"
#include "CvFGDStatModelParams.h"

#define CVFGDSTATMODEL_NAME "CvFGDStatModel"
#define checkCvFGDStatModel(L,i) luacv_checkObject<CvFGDStatModel>(L,i,CVFGDSTATMODEL_NAME)
#define pushCvFGDStatModel(L,data) luacv_pushObject<CvFGDStatModel>(L,data,CVFGDSTATMODEL_NAME,false)

extern const struct luaL_Reg CvFGDStatModel_m[];

#endif
