#ifndef CVPOSITOBJECT_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#define CVPOSITOBJECT_NAME "CvPOSITObject"

#define checkCvPOSITObject(L,i) luacv_checkObject<CvPOSITObject>(L,i,CVPOSITOBJECT_NAME)
void pushCvPOSITObject(lua_State *L,CvPOSITObject *);

extern const struct luaL_Reg CvPOSITObject_m[];
#endif
