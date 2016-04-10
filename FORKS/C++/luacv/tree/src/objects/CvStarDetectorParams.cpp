#include "CvStarDetectorParams.h"

static int CvStarDetectorParams_tostring(lua_State *L)
{
  CvStarDetectorParams *p=checkCvStarDetectorParams(L,1);
  lua_pushfstring(L,CVSTARDETECTORPARAMS_NAME" object: %p\n\tint maxSize=%d\n\tint responseThreshold=%d\n\tint lineThresholdProjected=%d\n\tlineThresholdBinarized=%d\n\tint suppressNonmaxSize=%d",p,p->maxSize,p->responseThreshold,p->lineThresholdProjected,p->lineThresholdBinarized,p->suppressNonmaxSize);
  return 1;
}

static int CvStarDetectorParams_gc(lua_State *L)
{
  luacv_obj<CvStarDetectorParams>*obj=(luacv_obj<CvStarDetectorParams>*)luaL_checkudata(L,1,CVSTARDETECTORPARAMS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvStarDetectorParams_nmaxSize(lua_State *L)
{
  const char f_msg[]=CVSTARDETECTORPARAMS_NAME".maxSize=int";
  checkCvStarDetectorParams(L,1)->maxSize=checkint(L,3);
return 0;
}

static int CvStarDetectorParams_maxSize(lua_State *L)
{
  lua_pushnumber(L,checkCvStarDetectorParams(L,1)->maxSize);
return 1;
}
static int CvStarDetectorParams_nresponseThreshold(lua_State *L)
{
  const char f_msg[]=CVSTARDETECTORPARAMS_NAME".responseThreshold=int";
  checkCvStarDetectorParams(L,1)->responseThreshold=checkint(L,3);
return 0;
}

static int CvStarDetectorParams_responseThreshold(lua_State *L)
{
  lua_pushnumber(L,checkCvStarDetectorParams(L,1)->responseThreshold);
return 1;
}
static int CvStarDetectorParams_nlineThresholdProjected(lua_State *L)
{
  const char f_msg[]=CVSTARDETECTORPARAMS_NAME".lineThresholdProjected=int";
  checkCvStarDetectorParams(L,1)->lineThresholdProjected=checkint(L,3);
return 0;
}

static int CvStarDetectorParams_lineThresholdProjected(lua_State *L)
{
  lua_pushnumber(L,checkCvStarDetectorParams(L,1)->lineThresholdProjected);
return 1;
}
static int CvStarDetectorParams_nlineThresholdBinarized(lua_State *L)
{
  const char f_msg[]=CVSTARDETECTORPARAMS_NAME".lineThresholdBinarized=int";
  checkCvStarDetectorParams(L,1)->lineThresholdBinarized=checkint(L,3);
return 0;
}

static int CvStarDetectorParams_lineThresholdBinarized(lua_State *L)
{
  lua_pushnumber(L,checkCvStarDetectorParams(L,1)->lineThresholdBinarized);
return 1;
}
static int CvStarDetectorParams_nsuppressNonmaxSize(lua_State *L)
{
  const char f_msg[]=CVSTARDETECTORPARAMS_NAME".suppressNonmaxSize=int";
  checkCvStarDetectorParams(L,1)->suppressNonmaxSize=checkint(L,3);
return 0;
}

static int CvStarDetectorParams_suppressNonmaxSize(lua_State *L)
{
  lua_pushnumber(L,checkCvStarDetectorParams(L,1)->suppressNonmaxSize);
return 1;
}
static const luacv_method CvStarDetectorParams_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(maxSize,CvStarDetectorParams),
  methodReg(responseThreshold,CvStarDetectorParams),
  methodReg(lineThresholdProjected,CvStarDetectorParams),
  methodReg(lineThresholdBinarized,CvStarDetectorParams),
  methodReg(suppressNonmaxSize,CvStarDetectorParams),
};

makeIndexFunctions(CvStarDetectorParams)
makeObjectCallback(CvStarDetectorParams);
