#ifndef LUACV_VIDEO

#define LUACV_VIDEO
#include "opencv2/opencv.hpp"
#include "luacvaux.h"
#include "lua_core.h"
#include "lua_imgproc.h"

#include "CvKalman.h"
#include "CvBGStatModel.h"
#include "CvFGDStatModelParams.h"
#include "CvBGPixelCStatTable.h"
#include "CvBGPixelCCStatTable.h"
#include "CvBGPixelStat.h"
#include "CvFGDStatModel.h"
#include "CvGaussBGStatModelParams.h"
#include "CvGaussBGValues.h"
#include "CvGaussBGPoint.h"
#include "CvGaussBGModel.h"
#include "CvBGCodeBookElem.h"
#include "CvBGCodeBookModel.h"


#define videoReg()\
  objectReg(CVKALMAN_NAME,CvKalman_m);\
  objectReg(CVBGSTATMODEL_NAME,CvBGStatModel_m);\
  objectReg(CVFGDSTATMODELPARAMS_NAME,CvFGDStatModelParams_m);\
  objectReg(CVBGPIXELCSTATTABLE_NAME,CvBGPixelCStatTable_m);\
  objectReg(CVBGPIXELCCSTATTABLE_NAME,CvBGPixelCCStatTable_m);\
  objectReg(CVBGPIXELSTAT_NAME,CvBGPixelStat_m);\
  objectReg(CVFGDSTATMODEL_NAME,CvFGDStatModel_m);\
  objectReg(CVGAUSSBGSTATMODELPARAMS_NAME,CvGaussBGStatModelParams_m);\
  objectReg(CVGAUSSBGVALUES_NAME,CvGaussBGValues_m);\
  objectReg(CVGAUSSBGPOINT_NAME,CvGaussBGPoint_m);\
  objectReg(CVGAUSSBGMODEL_NAME,CvGaussBGModel_m);\
  objectReg(CVBGCODEBOOKELEM_NAME,CvBGCodeBookElem_m);\
  objectReg(CVBGCODEBOOKMODEL_NAME,CvBGCodeBookModel_m);


extern const luaL_Reg video[];
extern const luacv_var video_var[];
extern const luacv_var video_object[];
#endif
