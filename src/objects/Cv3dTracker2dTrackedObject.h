#ifndef CV3DTRACKER2DTRACKEDOBJECT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvPoint2D32f.h"

#define CV3DTRACKER2DTRACKEDOBJECT_NAME "Cv3dTracker2dTrackedObject"

#define checkCv3dTracker2dTrackedObject(L,i) luacv_checkObject<Cv3dTracker2dTrackedObject>(L,i,CV3DTRACKER2DTRACKEDOBJECT_NAME)
#define pushCv3dTracker2dTrackedObject(L,data) luacv_pushObject<Cv3dTracker2dTrackedObject>(L,data,CV3DTRACKER2DTRACKEDOBJECT_NAME,true)

extern const struct luaL_Reg Cv3dTracker2dTrackedObject_m[];

#endif
