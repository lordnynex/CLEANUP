#ifndef CVFACETRACKER_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVFACETRACKER_NAME "CvFaceTracker"

#define checkCvFaceTracker(L,i) luacv_checkObject<CvFaceTracker>(L,i,CVFACETRACKER_NAME)
void pushCvFaceTracker(lua_State *L,CvFaceTracker *);

extern const struct luaL_Reg CvFaceTracker_m[];
#endif
