#include "CvSURFParams.h"

static int CvSURFParams_tostring(lua_State *L)
{
  CvSURFParams *params=checkCvSURFParams(L,1);
  lua_pushfstring(L,CVSURFPARAMS_NAME" object %p\n\tint extended=%d\n\tint upright=%d\n\tnum hessianThreshold=%f\n\tint nOctaves=%d\n\tint nOctaveLayers=%d",params,params->extended,params->upright,params->hessianThreshold,params->nOctaves,params->nOctaveLayers);
  return 1;
}

static int CvSURFParams_gc(lua_State *L)
{
  luacv_obj<CvSURFParams>*obj=(luacv_obj<CvSURFParams>*)luaL_checkudata(L,1,CVSURFPARAMS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvSURFParams_nhessianThreshold(lua_State *L)
{
  const char f_msg[]=CVSURFPARAMS_NAME".hessianThreshold=num";
  checkCvSURFParams(L,1)->hessianThreshold=checknumber(L,3);
return 0;
}

static int CvSURFParams_hessianThreshold(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFParams(L,1)->hessianThreshold);
return 1;
}

static int CvSURFParams_nextended(lua_State *L)
{
  const char f_msg[]=CVSURFPARAMS_NAME".extended=int";
  checkCvSURFParams(L,1)->extended=checkint(L,3);
return 0;
}

static int CvSURFParams_extended(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFParams(L,1)->extended);
return 1;
}

static int CvSURFParams_nupright(lua_State *L)
{
  const char f_msg[]=CVSURFPARAMS_NAME".upright=int";
  checkCvSURFParams(L,1)->upright=checkint(L,3);
return 0;
}

static int CvSURFParams_upright(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFParams(L,1)->upright);
return 1;
}

static int CvSURFParams_nnOctaves(lua_State *L)
{
  const char f_msg[]=CVSURFPARAMS_NAME".nOctaves=int";
  checkCvSURFParams(L,1)->nOctaves=checkint(L,3);
return 0;
}

static int CvSURFParams_nOctaves(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFParams(L,1)->nOctaves);
return 1;
}

static int CvSURFParams_nnOctaveLayers(lua_State *L)
{
  const char f_msg[]=CVSURFPARAMS_NAME".nOctaveLayers=int";
  checkCvSURFParams(L,1)->nOctaveLayers=checkint(L,3);
return 0;
}

static int CvSURFParams_nOctaveLayers(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFParams(L,1)->nOctaveLayers);
return 1;
}

static const luacv_method CvSURFParams_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(extended,CvSURFParams),
  methodReg(hessianThreshold,CvSURFParams),
  methodReg(nOctaves,CvSURFParams),
  methodReg(nOctaveLayers,CvSURFParams),
  methodReg(upright,CvSURFParams),
};

makeIndexFunctions(CvSURFParams)
makeObjectCallback(CvSURFParams);

