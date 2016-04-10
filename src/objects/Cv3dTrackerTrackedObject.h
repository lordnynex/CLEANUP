#ifndef CV3DTRACKERTRACKEDOBJECT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvPoint3D32f.h"

#define CV3DTRACKERTRACKEDOBJECT_NAME "Cv3dTrackerTrackedObject"

#define checkCv3dTrackerTrackedObject(L,i) luacv_checkObject<Cv3dTrackerTrackedObject>(L,i,CV3DTRACKERTRACKEDOBJECT_NAME)
#define pushCv3dTrackerTrackedObject(L,data) luacv_pushObject<Cv3dTrackerTrackedObject>(L,data,CV3DTRACKERTRACKEDOBJECT_NAME,true)

extern const struct luaL_Reg Cv3dTrackerTrackedObject_m[];

#endif
