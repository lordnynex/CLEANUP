#ifndef CVBGSTATMODEL_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"
#include "lua_core.h"

#define CVBGSTATMODEL_NAME "CvBGStatModel"
#define checkCvBGStatModel(L,i) luacv_checkObject<CvBGStatModel>(L,i,CVBGSTATMODEL_NAME)
#define pushCvBGStatModel(L,data) luacv_pushObject<CvBGStatModel>(L,data,CVBGSTATMODEL_NAME,false)

extern const struct luaL_Reg CvBGStatModel_m[];

#endif
