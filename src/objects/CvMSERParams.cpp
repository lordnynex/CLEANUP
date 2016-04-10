#include "CvMSERParams.h"

static int CvMSERParams_tostring(lua_State *L)
{
  CvMSERParams *params=checkCvMSERParams(L,1);
  lua_pushfstring(L,CVMSERPARAMS_NAME" object %p\n\tint delta=%d\n\tint maxArea=%d\n\tint minArea=%d\n\tnum maxVariation=%f\n\tnum minDiversity=%f\n\tint maxEvolution=%d\n\tnum areaThreshold=%f\n\tnum minMargin=%f\n\tint edgeBlurSize=%d",params,params->delta,params->maxArea,params->minArea,params->maxVariation,params->minDiversity,params->maxEvolution,params->areaThreshold,params->minMargin,params->edgeBlurSize);
  return 1;
}

static int CvMSERParams_gc(lua_State *L)
{
  luacv_obj<CvMSERParams>*obj=(luacv_obj<CvMSERParams>*)luaL_checkudata(L,1,CVMSERPARAMS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvMSERParams_nareaThreshold(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".areaThreshold=num";
  checkCvMSERParams(L,1)->areaThreshold=checknumber(L,3);
return 0;
}

static int CvMSERParams_areaThreshold(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->areaThreshold);
return 1;
}

static int CvMSERParams_nminMargin(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".minMargin=num";
  checkCvMSERParams(L,1)->minMargin=checknumber(L,3);
return 0;
}

static int CvMSERParams_minMargin(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->minMargin);
return 1;
}

static int CvMSERParams_nmaxVariation(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".maxVariation=num";
  checkCvMSERParams(L,1)->maxVariation=(float)checknumber(L,3);
return 0;
}

static int CvMSERParams_maxVariation(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->maxVariation);
return 1;
}
static int CvMSERParams_nminDiversity(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".minDiversity=num";
  checkCvMSERParams(L,1)->minDiversity=(float)checknumber(L,3);
return 0;
}

static int CvMSERParams_minDiversity(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->minDiversity);
return 1;
}

static int CvMSERParams_ndelta(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".delta=int";
  checkCvMSERParams(L,1)->delta=checkint(L,3);
return 0;
}

static int CvMSERParams_delta(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->delta);
return 1;
}

static int CvMSERParams_nmaxArea(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".maxArea=int";
  checkCvMSERParams(L,1)->maxArea=checkint(L,3);
return 0;
}

static int CvMSERParams_maxArea(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->maxArea);
return 1;
}
static int CvMSERParams_nminArea(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".minArea=int";
  checkCvMSERParams(L,1)->minArea=checkint(L,3);
return 0;
}

static int CvMSERParams_minArea(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->minArea);
return 1;
}
static int CvMSERParams_nmaxEvolution(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".maxEvolution=int";
  checkCvMSERParams(L,1)->maxEvolution=checkint(L,3);
return 0;
}

static int CvMSERParams_maxEvolution(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->maxEvolution);
return 1;
}

static int CvMSERParams_nedgeBlurSize(lua_State *L)
{
  const char f_msg[]=CVMSERPARAMS_NAME".edgeBlurSize=int";
  checkCvMSERParams(L,1)->edgeBlurSize=checkint(L,3);
return 0;
}

static int CvMSERParams_edgeBlurSize(lua_State *L)
{
  lua_pushnumber(L,checkCvMSERParams(L,1)->edgeBlurSize);
return 1;
}

static const luacv_method CvMSERParams_v[]=
{
  {NULL,10,NULL,NULL},
  methodReg(delta,CvMSERParams),
  methodReg(maxArea,CvMSERParams),
  methodReg(minArea,CvMSERParams),
  methodReg(maxVariation,CvMSERParams),
  methodReg(minDiversity,CvMSERParams),
  methodReg(maxEvolution,CvMSERParams),
  methodReg(areaThreshold,CvMSERParams),
  methodReg(minMargin,CvMSERParams),
  methodReg(edgeBlurSize,CvMSERParams),
};

makeIndexFunctions(CvMSERParams)
makeObjectCallback(CvMSERParams);

