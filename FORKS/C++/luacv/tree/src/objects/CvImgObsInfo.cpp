#include "CvImgObsInfo.h"

static int CvImgObsInfo_tostring(lua_State *L)
{
  CvImgObsInfo *p=checkCvImgObsInfo(L,1);
  lua_pushfstring(L,CVIMGOBSINFO_NAME" object: %p\n\tnum obs_x=%d\n\tnum obs_y=%d\n\tnum obs_size=%d\n\tnum[] obs=%p\n\tnum[] state=%p\n\tnum[] mix=%p",p,p->obs_x,p->obs_y,p->obs_size,p->obs,p->state,p->mix);
  return 1;
}

static int CvImgObsInfo_gc(lua_State *L)
{
  luacv_obj<CvImgObsInfo>*obj=(luacv_obj<CvImgObsInfo>*)luaL_checkudata(L,1,CVIMGOBSINFO_NAME);
  if (obj->dealloc_data)
    cvReleaseObsInfo(&(obj->data)); 
  return 0;
}

static int CvImgObsInfo_nobs_x(lua_State *L)
{
  const char f_msg[]=CVIMGOBSINFO_NAME".obs_x=int";
  checkCvImgObsInfo(L,1)->obs_x=checkint(L,3);
return 0;
}

static int CvImgObsInfo_obs_x(lua_State *L)
{
  lua_pushnumber(L,checkCvImgObsInfo(L,1)->obs_x);
return 1;
}

static int CvImgObsInfo_nobs_y(lua_State *L)
{
  const char f_msg[]=CVIMGOBSINFO_NAME".obs_y=int";
  checkCvImgObsInfo(L,1)->obs_y=checkint(L,3);
return 0;
}

static int CvImgObsInfo_obs_y(lua_State *L)
{
  lua_pushnumber(L,checkCvImgObsInfo(L,1)->obs_y);
return 1;
}

static int CvImgObsInfo_nobs_size(lua_State *L)
{
  const char f_msg[]=CVIMGOBSINFO_NAME".obs_size=int";
  checkCvImgObsInfo(L,1)->obs_size=checkint(L,3);
return 0;
}

static int CvImgObsInfo_obs_size(lua_State *L)
{
  lua_pushnumber(L,checkCvImgObsInfo(L,1)->obs_size);
return 1;
}

static const luacv_method CvImgObsInfo_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(obs_x,CvImgObsInfo),
  methodReg(obs_y,CvImgObsInfo),
  methodReg(obs_size,CvImgObsInfo),

};

makeIndexFunctions(CvImgObsInfo)
makeObjectCallback(CvImgObsInfo);
