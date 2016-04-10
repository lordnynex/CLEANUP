#include "opencv2/opencv.hpp"
#include "luacvaux.h"
#include "lua_core.h"

#include "CvPOSITObject.h"
#include "CvStereoBMState.h"
#include "CvStereoGCState.h"


#define calib3dReg()\
  objectReg(CVPOSITOBJECT_NAME,CvPOSITObject_m);\
  objectReg(CVSTEREOBMSTATE_NAME,CvStereoBMState_m);\
  objectReg(CVSTEREOGCSTATE_NAME,CvStereoGCState_m);\


extern const luaL_Reg calib3d[];
extern const luacv_var calib3d_var[];
extern const luacv_var calib3d_object[];
