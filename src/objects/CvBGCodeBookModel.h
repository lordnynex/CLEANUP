#ifndef CVBGCODEBOOKMODEL_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvMemStorage.h"
#include "CvBGCodeBookElem.h"
#include "CvSize.h"

#define CVBGCODEBOOKMODEL_NAME "CvBGCodeBookModel"

#define checkCvBGCodeBookModel(L,i) luacv_checkObject<CvBGCodeBookModel>(L,i,CVBGCODEBOOKMODEL_NAME)
#define pushCvBGCodeBookModel(L,data) luacv_pushObject<CvBGCodeBookModel>(L,data,CVBGCODEBOOKMODEL_NAME,false)

extern const struct luaL_Reg CvBGCodeBookModel_m[];
#endif
