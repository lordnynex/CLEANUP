#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "luacvaux.h"
#include "lua_core.h"


#include "CvImgObsInfo.h"
#include "CvEHMM.h"
#include "CvContourTree.h"
#include "CvGLCM.h"
#include "CvFaceTracker.h"
#include "Cv3dTracker2dTrackedObject.h"
#include "Cv3dTrackerTrackedObject.h"

#define legacyReg()\
  objectReg(CVIMGOBSINFO_NAME,CvImgObsInfo_m);\
  objectReg(CVEHMM_NAME,CvEHMM_m);\
  objectReg(CVCONTOURTREE_NAME,CvContourTree_m);\
  objectReg(CVGLCM_NAME,CvGLCM_m);\
  objectReg(CVFACETRACKER_NAME,CvFaceTracker_m);\
  objectReg(CV3DTRACKER2DTRACKEDOBJECT_NAME,Cv3dTracker2dTrackedObject_m);\
  objectReg(CV3DTRACKERTRACKEDOBJECT_NAME,Cv3dTrackerTrackedObject_m);\


extern const luaL_Reg legacy[];
extern const luacv_var legacy_var[];
extern const luacv_var legacy_object[];
